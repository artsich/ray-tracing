#pragma once

#include "material.h"

class dialectric : public material { 
public:
	dialectric(double refracted_coeff) 
		: refracted_coeff(refracted_coeff) 
		, col(color(1.0))
	{ }

	virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered_ray) override {
		attenuation = this->col;
		double etai_over_etat = refracted_coeff;
		if (rec.front_face) {
			etai_over_etat = 1.0 / refracted_coeff;
		}

		vec3 unit_direction = unit_vector(r.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

		if (etai_over_etat * sin_theta > 1.0) { 
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered_ray = ray(rec.p, reflected);
			return true;
		}

		double reflect_prob = schlink(cos_theta, etai_over_etat);
		if(random_double() < reflect_prob) {
			vec3 refracted = reflect(unit_direction, rec.normal);
			scattered_ray = ray(rec.p, refracted);
			return true;
		}

		vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
		scattered_ray = ray(rec.p, refracted);
		return true;
	}

private:
	double refracted_coeff;
	color col;
};