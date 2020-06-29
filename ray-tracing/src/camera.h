#pragma once

#include "vec3.h"

class camera
{
public:
    camera(const point3& origin, double focal_length, double viewport_height, double aspect_ratio)
        : origin(origin)
    {
        double viewport_width = aspect_ratio * viewport_height;
        horizontal = vec3(viewport_width, 0.0, 0.0);
        vertical = vec3(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - vec3(0, 0, focal_length);
    }

    inline ray get_ray(double u, double v) const
    {
        return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};
