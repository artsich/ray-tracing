#include <assert.h>

#include "common.h"
#include "camera.h"
#include "frame.h"

#include "shapes/sphere.h"
#include "shapes/hittable_list.h"

#include "materials/random_diffuse.h"
#include "materials/metal.h"

#include "system/concurrency/thread.h"

#include "command_line_args.h"
#include "util/benchmark.h"

#include "scene_parser/lexer.h"

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

void half_renderer(
	frame& iframe,
	const camera& camera,
	const hittable& world,
	int samples_per_pixel,
	int y_start, int y_end
) {
	auto width = iframe.get_width();
	auto height = iframe.get_height();

	y_end = y_end > height ? height : y_end;
	y_start = y_start < 0 ? 0 : y_start;

	for (int y = y_end - 1; y >= y_start; y--) {
		for (int x = 0; x < width; ++x) {
			color pixel_color(0.0, 0.0, 0.0);

			for (int s = 0; s < samples_per_pixel; ++s) {
				double u = double(x + random_double()) / (width - 1);
				double v = double(y + random_double()) / (height - 1);

				ray r = camera.get_ray(u, v);
				pixel_color += cast_ray(r, world);
			}

			iframe.write_color(x, y, pixel_color, samples_per_pixel);
		}
	}
}

struct render_async_params {
	int y_start;
	int y_end;
	int samples_per_pixel;
	frame* iframe;
	camera* camera;
	hittable* world;
};

render_async_params* generate_async_render_param(
	frame* iframe,
	camera* camera,
	hittable* world,
	int samples_per_pixel,
	int count
) { 
	render_async_params* params = new render_async_params[count];

	int height = iframe->get_height();
	params[0].camera = camera;
	params[0].world = world;
	params[0].iframe = iframe;
	params[0].samples_per_pixel = samples_per_pixel;
	params[0].y_start = 0;
	params[0].y_end = height * (1 / count);

	for(int i = 1; i < count; ++i) {
		params[i].camera = camera;
		params[i].world = world;
		params[i].iframe = iframe;
		params[i].samples_per_pixel = samples_per_pixel;

		params[i].y_start = params[i - 1].y_end;
		params[i].y_end = height * (int)(i / ((double)count - 1));
	}

	return params;
}

THREAD_PROCEDURE(render_async) {
	assert(args);
	render_async_params* params = (render_async_params*)args;

	if (params) {
		half_renderer(
			*params->iframe, 
			*params->camera, 
			*params->world, 
			params->samples_per_pixel, 
			params->y_start, params->y_end);

		return 1;
	}

	return 0;
}

int main(int argc, char** argv)
{

	lex_test();

	command_line_args cmd_args(argc, argv);

	int image_width = cmd_args.width;
	cmd_args.num_of_threads = 10;

	const auto aspect_ratio = 16.0 / 9.0;
	const auto image_height = static_cast<int>(image_width / aspect_ratio);

	const double viewport_height = 2.0;

	vec3 vup(0.0, 1.0, 0.0);
//	point3 lookfrom(-2.0, 2.0, 0.0);
	point3 lookfrom(0.0, 0.0, 0.0);
	point3 lookat(0.0, 0.0, -1.0);

	camera cam(lookfrom, lookat, vup, viewport_height, aspect_ratio, 90.0);

	hittable_list world;
	generate_world(world);

	frame image_frame(image_width, image_height);

	benchmark _benchmark;
	_benchmark.start();
	{
		if (cmd_args.num_of_threads > 1) {
			const int count_threads = cmd_args.num_of_threads;

			render_async_params* params = generate_async_render_param(
				&image_frame,
				&cam,
				&world,
				cmd_args.samples_per_pixel,
				count_threads);

			tid *threads = new tid[count_threads];
			for(int i = 0; i < count_threads; ++i) {
				threads[i] = create_thread(&render_async, &params[i], true);
			}

			for(int i = 0; i < count_threads; ++i) {
				join_thread(threads[i]);
				close_thread(threads[i]);
			}

			delete[] threads;
			delete params;
		}
		else {
			half_renderer(image_frame, cam, world, cmd_args.samples_per_pixel, 0, image_height);
		}
	}

	auto render_to_buffer_benchmark = _benchmark.stop();
	render_to_buffer_benchmark.log("\nRender done.\n", std::cerr);

	{
		_benchmark.start();
		image_frame.save_to_ppm(std::cout);
		auto load_to_file_benchmark = _benchmark.stop();
		load_to_file_benchmark.log("Save to file done.\n", std::cerr);
	}
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
			make_shared<metal>(color(.8,.6,.2), 0.1)
		)
	);

	world.add(
		make_shared<sphere>(
			point3(-1,0,-1),
			0.5,
			make_shared<metal>(color(.8,.8,.8), 1.0)
		)
	);

	world.add(
		make_shared<sphere>(
			point3(0,100.5,-2), 
			100, 
			make_shared<random_diffuse>(color(0.8, 0.8, 0.0))
		)
	);
}