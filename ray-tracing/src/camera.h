#pragma once

#include "common.h"
#include "vec3.h"

class camera
{
public:
	camera(
		const point3& lookfrom,
		const point3& lookat,
		const vec3& vup,
	 	double viewport_height,
	  	double aspect_ratio, 
		double vfov
	) 
		: origin(lookfrom)
	{
		double theta = degrees_to_radians(vfov);
		double h = tan(theta/2.0);
		viewport_height *= h;

		double viewport_width = aspect_ratio * viewport_height;

		vec3 forward = unit_vector(lookfrom - lookat);
		vec3 u = unit_vector(cross(vup, forward));
		vec3 v = cross(forward, u);

		horizontal = viewport_width * u;
		vertical = viewport_height * v;

		lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - forward;
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
