#include <chrono>

#include "common.h"

#include "camera.h"
#include "color.h"

#include "materials/lambertian.h"
#include "shapes/hittable_list.h"
#include "shapes/sphere.h"

const color white_color = color(1.0, 1.0, 1.0);
const color blue_color = color(0.5, 0.7, 1.0);
const color black_color = color(0.0, 0.0, 0.0);

inline color sky_color(const ray& r)
{
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y + 1.0);
    return lerp(white_color, blue_color, t);
}

color ray_color(const ray& r, const hittable& world, int depth = 20)
{
    if (depth < 0) 
    {
        return black_color;
    }

    hit_record rec;
    if (world.hit(r, epsilon, infinity, rec)) 
    {
        color col;
        ray scatered_ray;
        if (rec.mat->scatter(r, rec, col, scatered_ray)) {
            return col * ray_color(scatered_ray, world, depth - 1);
        }

        return black_color;
    }

    return sky_color(r);
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
    world.add(
        make_shared<sphere>(
            point3(0,0,-1), 
            0.5, 
            make_shared<lambertian>(color(1.0, 0.0, 0.0))
        )
    );

    world.add(
        make_shared<sphere>(
            point3(0,-100.5,-2), 
            100, 
            make_shared<lambertian>(color(0.0, 1.0, 0.0))
        )
    );

    auto start_time = std::chrono::system_clock::now();

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
    auto end_time = std::chrono::system_clock::now();
    
    double ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    double sec = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    
    std::cerr << "\nDone.\n" << "MS elapsed: " << ms << "\nSec: " << sec << "\n";
}
