#pragma once

#include <vector>
#include "aabb.h"
#include "hittable.h"
#include "rt_common.h"

class hittable_list : public hittable
{
    private:
    aabb bbox;
    
    public:
        std::vector<std::shared_ptr<hittable>> objects;
        hittable_list();
        hittable_list(std::shared_ptr<hittable> object);
        void clear();
        void add(std::shared_ptr<hittable> object);
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
        aabb bounding_box() const override;
};