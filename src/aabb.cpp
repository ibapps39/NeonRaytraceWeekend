#include "aabb.h"

// private:
void aabb::pad_to_minimums()
{
    if (x.size() < delta) { x = x.expand(delta); }
    if (y.size() < delta) { y = y.expand(delta); }
    if (z.size() < delta) { z = z.expand(delta); }
}
// public
aabb::aabb() {}
aabb::aabb(const interval& x, const interval& y, const interval& z): x(x), y(y), z(z)
{
    pad_to_minimums();
}

aabb::aabb(const point3& a, const point3& b)
{
    x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
    y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
    z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);

    pad_to_minimums();
}
aabb::aabb(const aabb& box_a, const aabb& box_b)
{
    x = interval(box_a.x, box_b.x);
    y = interval(box_a.y, box_b.y);
    z = interval(box_a.z, box_b.z);
}
const interval& aabb::axis_interval (int n) const
{
    if (n == 1) return y;
    if (n == 2) return z;
    return x;
}

bool aabb::hit(const ray& r, interval ray_t) const
{
    const point3& ray_orig = r.origin();
    const point3& ray_dir = r.direction();

    for (int axis = 0; axis < 3; axis++) 
    {
        const interval& current_axis = axis_interval(axis);
        const float adinv = 1.0 / ray_dir[axis];

        // t0 = intersection, t1 = second intersection
        float t0 = (current_axis.min - ray_orig[axis]) * adinv;
        float t1 = (current_axis.max - ray_orig[axis]) * adinv;

        // if current axis's min < max
        if (t0 < t1)
        {
            if (t0 > ray_t.min) ray_t.min = t0;
            if (t1 < ray_t.max) ray_t.max = t1;
        }
        else 
        {
            if (t1 > ray_t.min) ray_t.min = t1;
            if (t0 < ray_t.max) ray_t.max = t0;
        }
        if (ray_t.max <= ray_t.min)
        {
            return false;
        }
    }
    return true;
}

// maybe max the axis enums?
int aabb::longest_axis() const 
{
    if (x.size() > y.size())
    {
        return x.size() > z.size() ? 0 : 2;
    } else 
    {
        return y.size() > z.size() ? 1 : 2;
    }
}

const aabb aabb::empty = aabb(interval(interval::empty), interval(interval::empty), interval(interval::empty));
const aabb aabb::universe = aabb(interval(interval::universe), interval(interval::universe), interval(interval::universe));

aabb operator+(const aabb& bbox, const vec3& offset) {
    return aabb(bbox.x + offset.x(), bbox.y + offset.y(), bbox.z + offset.z());
}

aabb operator+(const vec3& offset, const aabb& bbox) {
    return bbox + offset;
}