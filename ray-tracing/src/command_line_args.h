#pragma once

#include <string>

struct command_line_args {
    int width;
    int is_debug_mode;
    int samples_per_pixel;

    command_line_args(int argc, char** argv) {
        if (argc == 4)
        {
            width = std::stoi(argv[1]);
            samples_per_pixel = std::stoi(argv[2]);
            is_debug_mode = std::stoi(argv[3]) == 1;
        }
        else 
        {
            width = 384;
            samples_per_pixel = 100;
            is_debug_mode = true;
        }
    }
};
