#pragma once
#include "rt_common.h"
#include "hittable.h"
#include "hittable_list.h"
class quad : public hittable 
{
    private:
        point3 Q;
        vec3 u, v, w, normal;
        float D;
        std::shared_ptr<material> mat;
        aabb bbox;

    public: 
        quad(const point3& Q, const vec3& u, const vec3& v, std::shared_ptr<material> mat);
        virtual void set_bounding_box()
        {
            aabb bbox_diagional1 = aabb(Q, Q + u + v);
            aabb bbox_diagional2 = aabb(Q + u, Q + v);
            bbox = aabb(bbox_diagional1, bbox_diagional2);
        }
        virtual bool is_interior(float a, float b, hit_record& rec) const;
        aabb bounding_box() const override;
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
};
inline std::shared_ptr<hittable_list> box(const point3& a, const point3& b, std::shared_ptr<material> mat)
{
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    auto sides = std::make_shared<hittable_list>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = point3(std::fmin(a.x(),b.x()), std::fmin(a.y(),b.y()), std::fmin(a.z(),b.z()));
    auto max = point3(std::fmax(a.x(),b.x()), std::fmax(a.y(),b.y()), std::fmax(a.z(),b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->add(std::make_shared<quad>(point3(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
    sides->add(std::make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
    sides->add(std::make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
    sides->add(std::make_shared<quad>(point3(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
    sides->add(std::make_shared<quad>(point3(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
    sides->add(std::make_shared<quad>(point3(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom

    return sides;
};