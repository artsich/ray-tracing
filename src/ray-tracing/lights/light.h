#pragma once

#include "vec3.h"

class light { 
public:
	virtual color emit(const point3 to, const vec3 normal, const color& color) = 0;
};