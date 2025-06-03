#include "bvh.h"

// Implicit list copy!
bvh_node::bvh_node(hittable_list list): bvh_node(list.objects, 0, list.objects.size())
{}
aabb bvh_node::bounding_box() const
{
    return bbox;
}
bool bvh_node::box_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis_index) 
{
    interval a_axis_interval = a->bounding_box().axis_interval(axis_index);
    interval b_axis_interval = b->bounding_box().axis_interval(axis_index);
    return a_axis_interval.min < b_axis_interval.min;
}
bool bvh_node::box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) 
{
    return box_compare(a, b, 0);
}
bool bvh_node::box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) 
{
    return box_compare(a, b, 1);
}
bool bvh_node::box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) 
{
    return box_compare(a, b, 2);
}

bvh_node::bvh_node(std::vector<std::shared_ptr<hittable>> &objects, size_t start, size_t end)
{
    bbox = aabb::empty;
    for (size_t object_index = start; object_index < end; object_index++)
    {
        bbox = aabb(bbox, objects[object_index]->bounding_box());
    }
    int axis = bbox.longest_axis();

    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        left = objects[start];
        right = objects[start+1];
    } else {
        std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

        auto mid = start + object_span/2;
        left = std::make_shared<bvh_node>(objects, start, mid);
        right = std::make_shared<bvh_node>(objects, mid, end);
    }
}
bool bvh_node::hit(const ray& r, interval ray_t, hit_record& rec) const
{
    if (!bbox.hit(r, ray_t))
    {
        return false;
    }

    bool hit_left = left->hit(r, ray_t, rec);
    bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);
    return hit_left || hit_right;
}
