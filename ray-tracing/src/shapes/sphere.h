#pragma once

#include "hittable.h"

class sphere : public hittable
{
public:

    sphere(const point3& position, double radius, shared_ptr<material> m)
        : radius(radius)
        , double_radius(radius* radius)
        , mat(m)
        , center(position)
    {
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
    {
        /*
            The disciminant formula was optimazed, b = 2h, where h = 2/b
        */

        vec3 oc = r.origin() - center;
        double a = r.direction().length_squared();
        double half_b = dot(oc, r.direction());
        double c = oc.length_squared() - double_radius;
        double descrimitant = half_b * half_b - a * c;

        if (descrimitant > 0)
        {
            double sqrt_d = sqrt(descrimitant);
            return select_root_if_in_interval(t_min, t_max, half_b, -sqrt_d, a, r, rec) ||
                select_root_if_in_interval(t_min, t_max, half_b, sqrt_d, a, r, rec);
        }

        return false;
    }

private:

    bool select_root_if_in_interval(
        double t_min, double t_max, 
        double half_b, double sqrt_descriminant, double a, 
        const ray& r, hit_record& rec) const
    {
        double root = (-half_b + sqrt_descriminant) / a;
        if (t_min < root && root < t_max)
        {
            fill_hit_record(root, r, rec);
            return true;
        }
        return false;
    }

    inline void fill_hit_record(double root, const ray& r, hit_record& rec) const
    {
        rec.t = root;
        rec.p = r.at(rec.t);
        rec.mat = mat;
        // This expression equal unit_lenght(rec.p - this->center)
        //
        // If you want to unarstand it, just plot this on papper!
        vec3 outward_normal = (rec.p - this->center) / radius;
        rec.set_face_normal(r, outward_normal);
    }

private:
    double radius;
    double double_radius;
    shared_ptr<material> mat;
    point3 center;
};
