#include "common.h"
#include "camera.h"
#include "frame.h"

#include "shapes/sphere.h"
#include "shapes/hittable_list.h"

#include "materials/random_diffuse.h"
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

color cast_ray(const ray& r, const hittable& world, int depth = 50) {
	if (depth < 0) {
		return black_color;
	}

	hit_record rec;
	if (world.hit(r, epsilon, infinity, rec)) 
	{
		ray scatered_ray;
		color col_attenuation;

		if (rec.mat->scatter(r, rec, col_attenuation, scatered_ray)) {
			return col_attenuation * cast_ray(scatered_ray, world, depth - 1);
		}

		// TODO: red just for debug, change on black_color
		return red_color;
	}

	return sky_color(r);
}

void render(
	frame& iframe,
	const camera& camera, 
	const hittable& world, 
	int samples_per_pixel,
	bool is_debug_mode
) {
	auto width = iframe.get_width();
	auto height = iframe.get_height();

	for(int y = height - 1; y >= 0; y--) {
		if (is_debug_mode)
			std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;

		for (int x = 0; x < width; ++x) {
			color pixel_color(0.0, 0.0, 0.0);

			for(int s = 0; s < samples_per_pixel; ++s) { 
				double u = double(x + random_double()) / (width - 1);
				double v = double(y + random_double()) / (height - 1);

				ray r = camera.get_ray(u, v);
				pixel_color += cast_ray(r, world);
			}

			iframe.write_color(x, y, pixel_color, samples_per_pixel);
		}
	}
}

int main(int argc, char** argv)
{
	command_line_args cmd_args(argc, argv);

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
		render(image_frame, cam, world, cmd_args.samples_per_pixel, cmd_args.is_debug_mode);
		image_frame.save_to_ppm(std::cout);
	}
	auto benchmark_result = _benchmark.stop();

	benchmark_result.log(std::cerr);
}

void generate_world(hittable_list& world) {
	world.add(
		make_shared<sphere>(
			point3(0,0,-2), 
			0.5, 
			make_shared<random_diffuse>(color(0.7, 0.3, 0.3))
		)
	);

	world.add(
		make_shared<sphere>(
			point3(1,0,-1.5), 
			0.5,
			make_shared<metal>(color(.8,.6,.2), 0)
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
			make_shared<metal>(color(0.8, 0.8, 0.0), 0)
		)
	);
}