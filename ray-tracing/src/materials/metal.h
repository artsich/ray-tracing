#pragma once

#include "material.h"

class metal : public material
{
public:
	metal(const color& albedo, double fuzz = 0.0)
		: albedo(albedo)
		, fuzz(fuzz)
	{
	}

	virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scatered_ray) override {
		vec3 reflected_ray_dir = reflect(rec.normal, unit_vector(r.direction()));
		scatered_ray = ray(rec.p, reflected_ray_dir + fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return dot(reflected_ray_dir, rec.normal) > 0.0;
	}

private:
	color albedo;
	double fuzz;
};