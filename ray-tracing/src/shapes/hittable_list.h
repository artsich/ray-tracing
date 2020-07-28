#pragma once

#include "hittable.h"

#include <memory>
#include <vector>
#include <algorithm>

#include "lights/light.h"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable
{
public:
	hittable_list() { }
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) { objects.push_back(object); }
	void add_light(shared_ptr<light> light) { lights.push_back(light); }

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override  
	{
		hit_record temp_rec;
		bool hit_anything = false;
		auto closest_so_far = t_max;
		for (const auto& object : objects)
		{
			if (object->hit(r, t_min, closest_so_far, temp_rec))
			{
				hit_anything = true;
				closest_so_far = std::min(closest_so_far, temp_rec.t);
				rec = temp_rec;
			}
		}

		return hit_anything;
	}

public:
	std::vector<shared_ptr<hittable>> objects;
	std::vector<shared_ptr<light>> lights;
};
