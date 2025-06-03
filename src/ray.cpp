#include "ray.h"
#include "color.h"
const point3& ray::origin() const  { return origin_; }
const vec3& ray::direction() const { return direction_; }

ray::ray()
{}
ray::ray(const point3& origin, const vec3& direction) : origin_(origin), direction_(direction)
{}
ray::ray(const point3& origin, const vec3& direction, float time) : origin_(origin), direction_(direction), time_(time)
{}
point3 ray::at(float t) const {
    return origin_ + t*direction_;
}
float ray::time() const {
    return time_;
}

  