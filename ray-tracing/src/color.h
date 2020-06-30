#pragma once

#include "vec3.h"

#include <iostream>

inline void gamma_correct(color& col, double scale) 
{
    col.r = sqrt(col.r * scale);
    col.g = sqrt(col.g * scale);
    col.b = sqrt(col.b * scale);
}

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) 
{
    auto scale = 1.0 / samples_per_pixel;
    color corrected_color = pixel_color;
    gamma_correct(corrected_color, scale);

    out << static_cast<int>(255.999 * clamp01(corrected_color.r)) << ' '
        << static_cast<int>(255.999 * clamp01(corrected_color.g)) << ' '
        << static_cast<int>(255.999 * clamp01(corrected_color.b)) << std::endl;
}
