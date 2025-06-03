#pragma once

#include "aabb.h"

class material;

class hit_record
{
    public:
        point3 p;
        vec3 normal;
        std::shared_ptr<material> mat;
        float t;
        float u;
        float v;
        bool front_face;
        void set_face_normal(const ray& r, const vec3& outward_normal);
};

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

    virtual aabb bounding_box() const = 0;
};

class translate : public hittable 
{
    private:
        std::shared_ptr<hittable> object;
        vec3 offset;
        aabb bbox;
  public:
    aabb bounding_box() const override;
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
    translate(std::shared_ptr<hittable> object, const vec3& offset);
};

class rotate_y : public hittable
{
    private:
        std::shared_ptr<hittable> object;
        float sin_theta;
        float cos_theta;
        aabb bbox;

    public:
        aabb bounding_box() const override;
        rotate_y(std::shared_ptr<hittable> object, float angle);
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
};