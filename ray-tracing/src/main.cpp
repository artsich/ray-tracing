#include "common.h"

#include "camera.h"
#include "color.h"

#include "shapes/hittable_list.h"
#include "shapes/sphere.h"

const color white_color = color(1.0, 1.0, 1.0);
const color blue_color = color(0.5, 0.7, 1.0);

inline color default_color(const ray& r)
{
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y + 1.0);
    return lerp(white_color, blue_color, t);
}

color ray_color(const ray& r, const hittable& world)
{
    hit_record rec;
    if (world.hit(r, 0.0, infinity, rec)) 
    {
        return 0.5 * (rec.normal + white_color);
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
    const int samples_per_pixel = 100;

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    const double viewport_height = 2.0;
    const double focal_length = 1.0;

    camera cam(point3(0.0, 0.0, 0.0), focal_length, viewport_height, aspect_ratio);

    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-2), 100));

    for (int j = image_height - 1; j >= 0; --j) {
        if (isDebugMode)
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i) 
        {
            color pixel_color;
            for (int s = 0; s < samples_per_pixel; ++s) 
            {
                auto u = double(i + random_double()) / (image_width - 1);
                auto v = double(j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);

            }

            if(isWrite)
                write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
