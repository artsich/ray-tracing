#pragma once

#include "hittable.h"

#include "vec3.h"

#include "materials/material.h"

class plane : public hittable { 
public:

	plane(const point3& pos, const vec3& n, double width, double height, std::shared_ptr<material> material) 
		: pos(pos)
		, normal(n)
		, half_width(width/2.0)
		, half_height(height/2.0)
		, material(material)
	{
	}

	virtual bool hit(const ray& ray, double t_min, double t_max, hit_record& rec) const override {
		double t;
		const vec3& ray_pos = ray.origin();
		const vec3& ray_dir = unit_vector(ray.direction());

		if (hit_infinity_plane(ray_pos, ray_dir, t)) {
			// from plane origin to intersect point 
			point3 intersect_point = ray.at(t);
			vec3 from_p0_to_p = intersect_point - this->pos;

			if (abs(from_p0_to_p.x) < this->half_width && abs(from_p0_to_p.y) < this->half_height) {
				fill_hit_rec(ray, rec, intersect_point, t);
				return true;
			}
		}

		return false;
	}

private:
	/* 
			dot((P0 - R0), * N)
		T =	-------------------
				dot(R, N)			// IF > 0 then intersect, else don't

		N - normal of plane,
		P0 - plane position

		R - ray dir,
		R0 - ray origin

		P = R0 + R*T; -- intersect point
	*/
	inline bool hit_infinity_plane(const vec3& R0, const vec3& R, double& T) const {
		const vec3& P0 = this->pos;
		const vec3& N = this->normal;

		double denominator = dot(R, N);
		if (denominator > 1e-6) {
			T = dot( P0 - R0, N ) / denominator;
			return true;
		}
		return false;
	}

	inline void fill_hit_rec(const ray& ray, hit_record& rec, const point3& intersect_point, double t) const { 
//		rec.normal = this->normal;
		rec.set_face_normal(ray, this->normal);
		rec.mat = this->material;
		rec.p = intersect_point;
		rec.t = t;
	}

private:
	double half_width, half_height;
	vec3 normal;
	point3 pos;
	std::shared_ptr<material> material;
};