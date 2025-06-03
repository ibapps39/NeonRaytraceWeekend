#pragma once
#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

#include<algorithm>

class bvh_node : public hittable
{
    private:
        std::shared_ptr<hittable> left;
        std::shared_ptr<hittable> right;
        aabb bbox;

    public:
        aabb bounding_box() const override;
        bvh_node(hittable_list list);
        static bool box_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis_index);
        static bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b);
        static bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b);
        static bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b);
        bvh_node(std::vector<std::shared_ptr<hittable>>& objects, size_t start, size_t end);
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
        
};
