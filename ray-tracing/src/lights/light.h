#pragma once

#include "vec3.h"

class light { 
public:
	light(const point3& pos, const vec3& dir, const color& color) 
		: pos(pos)
		, dir(dir)
		, color(color)
	{}

	color emit(const point3 to, const vec3 normal, const color& color) { 
	} 

private:
	vec3 dir;
	point3 pos;
	color color;
};