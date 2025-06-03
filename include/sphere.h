#pragma once
#include "hittable.h"

class sphere : public hittable
{
private:
    point3 center_pos;
    float radius;
    std::shared_ptr<material> mat;
    ray displacement_ray;
    aabb bbox;
    static void get_sphere_uv(const point3& p, float& u, float& v);

public:
    sphere(const point3& static_center, float radius, std::shared_ptr<material> mat);
    // Moving Sphere
    sphere(const point3& center1, const point3 &center2, float radius, std::shared_ptr<material> mat);
    sphere(const point3& p, float& u, float& v);

    aabb bounding_box() const override;
    bool hit(const ray &r, interval ray_t, hit_record &hit_rec) const override;
};
float hit_sphere(const point3 &center, float radius, const ray &r);
