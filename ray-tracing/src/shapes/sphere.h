#pragma once

#include "hitable.h"

class sphere : public hitable
{
public:

    sphere(const point3& position, double radius, color color)
        : center(position)
        , radius(radius)
        , col(color)
        , double_radius(radius* radius)
    {
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) override
    {
        /*
            The disciminant formula was optimazed, b = 2h, where h = 2/b
        */

        vec3 oc = r.origin() - center;
        double a = dot(r.direction(), r.direction());
        double half_b = dot(oc, r.direction());
        double c = dot(oc, oc) - double_radius;
        double descrimitant = half_b * half_b - a * c;

        if (descrimitant > 0)
        {
            double sqrt_d = sqrt(descrimitant);
            return select_root_if_in_interval(t_min, t_max, half_b, sqrt_d, a, r, rec) ||
                select_root_if_in_interval(t_min, t_max, half_b, -sqrt_d, a, r, rec);
        }

        return false;
    }

    inline virtual vec3 get_pos() override { return center; };

    const color& get_color() const { return col; }

private:

    bool select_root_if_in_interval(
        double t_min, double t_max, 
        double half_b, double sqrt_descriminant, double a, 
        const ray& r, hit_record& rec)
    {
        double root = (-half_b + sqrt_descriminant) / a;
        if (t_min < root && root < t_max)
        {
            fill_hit_record(root, r, rec);
            return true;
        }
        return false;
    }

    inline void fill_hit_record(double root, const ray& r, hit_record& rec)
    {
        rec.t = root;
        rec.p = r.at(rec.t);

        // This expression equal unit_lenght(rec.p - this->center)
        //
        // If you want to unarstand it, just plot this on papper!
        vec3 outward_normal = (rec.p - this->center) / radius;
        rec.set_face_normal(r, outward_normal);
    }

private:
    point3 center;
    color col;
    double radius;
    double double_radius;
};
