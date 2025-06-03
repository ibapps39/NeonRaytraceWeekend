#pragma once

#include "hittable.h"
#include "material.h"
#include "texture.h"

class constant_medium : public hittable
{
    private:
    std::shared_ptr<hittable> boundary;
    float neg_inv_density;
    std::shared_ptr<material> phase_function;

    public:
        aabb bounding_box() const override;
        constant_medium(std::shared_ptr<hittable> boundary, float density, std::shared_ptr<texture> tex);
        constant_medium(std::shared_ptr<hittable> boundary, float density, const color& albedo);
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
};