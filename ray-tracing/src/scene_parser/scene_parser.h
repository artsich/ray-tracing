#pragma once

#include "common.h"

#include "camera.h"
#include "shapes/sphere.h"
#include "shapes/hittable_list.h"

struct scene { 
	camera camera;
	std::shared_ptr<hittable> world;
};

void parse_scene(const char* scene) { 
	
}