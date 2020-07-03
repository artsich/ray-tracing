#include "common.h"
#include "camera.h"
#include "frame.h"

#include "shapes/sphere.h"
#include "shapes/hittable_list.h"

#include "materials/lambertian.h"
#include "materials/metal.h"

#include "command_line_args.h"
#include "util/benchmark.h"

void generate_world(hittable_list& world);

const color white_color = color(1.0, 1.0, 1.0);
const color blue_color = color(0.5, 0.7, 1.0);
const color black_color = color(0.0, 0.0, 0.0);
const color red_color = color(1.0, 0.0, 0.0);

inline color sky_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y + 1.0);
    return lerp(white_color, blue_color, t);
}

color ray_color(const ray& r, const hittable& world, int depth = 50) {
    if (depth < 0) {
        return black_color;
    }

    hit_record rec;
    if (world.hit(r, epsilon, infinity, rec)) 
    {
        ray scatered_ray;
        color col_attenuation;

        if (rec.mat->scatter(r, rec, col_attenuation, scatered_ray)) {
            return col_attenuation * ray_color(scatered_ray, world, depth - 1);
        }

        // TODO: red just for debug, change on black_color
        return red_color;
    }

    return sky_color(r);
}

int main(int argc, char** argv)
{
    command_line_args cmd_args(argc, argv);

    int samples_per_pixel = cmd_args.samples_per_pixel;
    int image_width = cmd_args.width;

    const auto aspect_ratio = 16.0 / 9.0;
    const auto image_height = static_cast<int>(image_width / aspect_ratio);

    const double viewport_height = 2.0;
    const double focal_length = 1.0;

    camera cam(point3(0.0, 0.0, 0.0), focal_length, viewport_height, aspect_ratio);

    hittable_list world;
    generate_world(world);

    frame image_frame(image_width, image_height);

    benchmark _benchmark;
    _benchmark.start();
    { 
        for (int j = image_frame.get_height() - 1; j >= 0; --j) {
            if (cmd_args.is_debug_mode)
                std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

            for (int i = 0; i < image_frame.get_width(); ++i) 
            {
                color pixel_color;
                for (int s = 0; s < samples_per_pixel; ++s) 
                {
                    auto u = double(i + random_double()) / (image_width - 1);
                    auto v = double(j + random_double()) / (image_height - 1);
                    ray r = cam.get_ray(u, v);
                    pixel_color += ray_color(r, world);
                }

                image_frame.write_color(i, j, pixel_color, samples_per_pixel);
            }
        }
    }
    image_frame.save_to_ppm(std::cout);
   
    auto benchmark_result = _benchmark.stop();
    benchmark_result.log(std::cerr);
}

void generate_world(hittable_list& world) {
    world.add(
        make_shared<sphere>(
            point3(0,0,-1), 
            0.5, 
            make_shared<lambertian>(color(0.7, 0.3, 0.3))
        )
    );

    world.add(
        make_shared<sphere>(
            point3(1,0,-1), 
            0.5,
            make_shared<metal>(color(.8,.6,.2), 0.5)
        )
    );

    world.add(
        make_shared<sphere>(
            point3(-1,0,-1),
            0.5,
            make_shared<metal>(color(.8,.8,.8), 1)
        )
    );

    world.add(
        make_shared<sphere>(
            point3(0,100.5,-2), 
            100, 
            make_shared<lambertian>(color(0.8, 0.8, 0.0))
        )
    );
}