#pragma once
#include "rt_common.h"
class aabb
{
    private:
    void pad_to_minimums();
        /*
            void pad_to_minimums() {
        // Adjust the AABB so that no side is narrower than some delta, padding if necessary.

        double delta = 0.0001;
        if (x.size() < delta) x = x.expand(delta);
        if (y.size() < delta) y = y.expand(delta);
        if (z.size() < delta) z = z.expand(delta);
    }
        */
    public:
        interval x,y,z;
        float delta = 0.00001f;
        static const aabb empty, universe;

        aabb();
        aabb(const interval& x, const interval& y, const interval& z);
        aabb(const point3& a, const point3& b);
        aabb(const aabb& box_a, const aabb& box_b);
        int longest_axis() const;

        const interval& axis_interval (int n) const;

        bool hit(const ray& r, interval ray_t) const;
};
aabb operator+(const aabb& bbox, const vec3& offset);

aabb operator+(const vec3& offset, const aabb& bbox);