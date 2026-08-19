#include "organise/organiser.hpp"
#include "organise/colors.hpp"
#include "organise/log.hpp"
#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <cerrno>
#include <cstring>

namespace organise {

static std::unordered_map<int, std::filesystem::path> wd_to_path;

static void addWatch(int fd, const std::filesystem::path& dir) {
    int wd = inotify_add_watch(fd, dir.c_str(),
                               IN_CLOSE_WRITE | IN_MOVED_TO | IN_CREATE);
    if (wd >= 0) {
        wd_to_path[wd] = dir;
        std::cout << color::cyan << "[WATCH] Now watching: " << dir.string()
                  << color::reset << std::endl;
    } else {
        std::cerr << color::red << "Failed to watch: " << dir.string()
                  << " (" << strerror(errno) << ")"
                  << color::reset << std::endl;
    }
}

static void addWatchRecursive(int fd, const std::filesystem::path& dir) {
    addWatch(fd, dir);
    if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir)) {
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (entry.is_directory()) {
                addWatchRecursive(fd, entry.path());
            }
        }
    }
}

void startWatcher(const nlohmann::json& rules, const Options& opts) {
    int fd = inotify_init();
    if (fd < 0) {
        std::cerr << color::red << "inotify_init failed: " << strerror(errno)
                  << color::reset << std::endl;
        return;
    }

    if (!std::filesystem::exists(opts.targetDir) ||
        !std::filesystem::is_directory(opts.targetDir)) {
        std::cerr << color::red << "Invalid directory: " << opts.targetDir.string()
                  << color::reset << std::endl;
        close(fd);
        return;
    }

    addWatchRecursive(fd, opts.targetDir);

    std::cout << color::blue << "Watcher active! Monitoring recursively: "
              << opts.targetDir.string() << color::reset << std::endl;
    std::cout << color::yellow << "Press Ctrl+C to exit." << color::reset << std::endl;

    log::logActivity("Watcher started on " + opts.targetDir.string());

    char buffer[4096] __attribute__((aligned(__alignof__(struct inotify_event))));

    while (true) {
        ssize_t len = read(fd, buffer, sizeof(buffer));
        if (len < 0) {
            std::cerr << color::red << "read() failed: " << strerror(errno)
                      << color::reset << std::endl;
            break;
        }

        for (char* ptr = buffer; ptr < buffer + len; ) {
            struct inotify_event* event = (struct inotify_event*) ptr;

            if (event->len > 0) {
                auto it = wd_to_path.find(event->wd);
                if (it == wd_to_path.end()) {
                    ptr += sizeof(struct inotify_event) + event->len;
                    continue;
                }

                std::filesystem::path base = it->second;
                std::filesystem::path fullPath = base / event->name;

                if (event->mask & IN_CREATE && (event->mask & IN_ISDIR)) {
                    addWatchRecursive(fd, fullPath);
                    log::logActivity("New directory watched: " + fullPath.string());
                }
                else if (event->mask & (IN_CLOSE_WRITE | IN_MOVED_TO)) {
                    usleep(500000); // wait for file to finish writing
                    if (std::filesystem::exists(fullPath) &&
                        std::filesystem::is_regular_file(fullPath)) {
                        handleFile(std::filesystem::directory_entry(fullPath), rules, opts);
                    }
                }
            }

            ptr += sizeof(struct inotify_event) + event->len;
        }
    }

    close(fd);
}

} // namespace organise
