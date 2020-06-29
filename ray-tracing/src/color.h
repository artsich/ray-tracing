#pragma once

#include "vec3.h"

#include <iostream>

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) 
{
    auto scale = 1.0 / samples_per_pixel;
    color new_color = pixel_color * scale;

    out << static_cast<int>(255.999 * new_color.r) << ' '
        << static_cast<int>(255.999 * new_color.g) << ' '
        << static_cast<int>(255.999 * new_color.b) << std::endl;
}
