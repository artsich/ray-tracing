#pragma once

#include <string>
#include "system/concurrency/thread.h"

struct command_line_args {
    int width;
    int is_debug_mode;
    int samples_per_pixel;
    int num_of_threads;

    command_line_args(int argc, char** argv) {
        if (argc == 5)
        {
            width = std::stoi(argv[1]);
            samples_per_pixel = std::stoi(argv[2]);
            is_debug_mode = std::stoi(argv[3]) == 1;
            num_of_threads = std::stoi(argv[4]);
        }
        else 
        {
            width = 384;
            samples_per_pixel = 100;
            is_debug_mode = true;
            num_of_threads = hardware_concurrency();
        }
    }
};
