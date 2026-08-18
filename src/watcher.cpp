#include "../include/organiser.hpp"
#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>

namespace organise {
void startWatcher(const nlohmann::json& rules, const Options& opts) {
    int fd = inotify_init();
    int wd = inotify_add_watch(fd, opts.targetDir.c_str(), IN_CLOSE_WRITE | IN_MOVED_TO);

    std::cout << color::blue << "Watcher active in: " << opts.targetDir.string() << color::reset << std::endl;

    char buffer[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
    
    while (true) {
        ssize_t len = read(fd, buffer, sizeof(buffer));
        for (char* ptr = buffer; ptr < buffer + len; ) {
            struct inotify_event* event = (struct inotify_event*) ptr;
            if (event->len > 0) {
                std::filesystem::path newFile = opts.targetDir / event->name;
                usleep(500000); 
                if (std::filesystem::exists(newFile)) {
                    handleFile(std::filesystem::directory_entry(newFile), rules, opts);
                }
            }
            ptr += sizeof(struct inotify_event) + event->len;
        }
    }
}
}
