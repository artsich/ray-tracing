#include "color.h"
#include "ray.h"

#include <vector>
#include <string>
#include "shapes/sphere.h"

std::vector<sphere> spheres = { 
    sphere(point3(0.0, 0.0, -2.0), 1, color(1.0, 0.0, 0.0))
    //,sphere(point3(2.0, 0.0, -2.0), 1, color(1.0, 0.0, 1.0))
};

inline color default_color(const ray& r)
{
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

color ray_color(const ray& r)
{
    for(auto& sphere : spheres)
    {
        hit_record rec;
        if (sphere.hit(r, 0.0, 10.0, rec))
        {
            vec3& N = rec.normal;
            return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
        }
    }

    return default_color(r);
}

int main(int argc, char** argv)
{
    int image_width = 384;
    bool isDebugMode = true;
    bool isWrite = false;

    if (argc == 3)
    {
        image_width = std::stoi(argv[1]);
        isDebugMode = std::stoi(argv[2]) == 1;
        isWrite = true;
    }

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    const auto viewport_height = 2.0;
    const auto viewport_width = aspect_ratio * viewport_height;
    const auto focal_length = 1.0;

    const auto origin = point3(0, 0, 0);
    const auto horizontal = vec3(viewport_width, 0, 0);
    const auto vertical = vec3(0, viewport_height, 0);
    const auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    for (int j = image_height - 1; j >= 0; --j) {
        if (isDebugMode)
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);

            if(isWrite)
                write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}
