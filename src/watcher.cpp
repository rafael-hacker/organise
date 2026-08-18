#include "../include/organiser.hpp"
#include "../include/colors.hpp"
#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <filesystem>

namespace organise {

void startWatcher(const nlohmann::json& rules, const Options& opts) {
    int fd = inotify_init();
    if (fd < 0) {
        std::cerr << color::red << "Error initializing inotify." << color::reset << std::endl;
        return;
    }

    int wd = inotify_add_watch(fd, opts.targetDir.c_str(), IN_CLOSE_WRITE | IN_MOVED_TO);
    if (wd < 0) {
        std::cerr << color::red << "Error while monitoring directory: " << opts.targetDir.string() << color::reset << std::endl;
        close(fd);
        return;
    }

    std::cout << color::blue << "Watcher active! Monitoring directory: " << opts.targetDir.string() << color::reset << std::endl;
    std::cout << color::yellow << "Press Ctrl+C to exit." << color::reset << std::endl;

    char buffer[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
    
    while (true) {
        ssize_t len = read(fd, buffer, sizeof(buffer));
        if (len < 0) break;

        for (char* ptr = buffer; ptr < buffer + len; ) {
            struct inotify_event* event = (struct inotify_event*) ptr;
            
            if (event->len > 0) {
                std::filesystem::path newFile = opts.targetDir / event->name;
                
                usleep(500000); 
                
                if (std::filesystem::exists(newFile) && std::filesystem::is_regular_file(newFile)) {
                    handleFile(std::filesystem::directory_entry(newFile), rules, opts);
                }
            }
            
            ptr += sizeof(struct inotify_event) + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}

} // namespace organise
