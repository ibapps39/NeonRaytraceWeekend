#include "onb.h"

//Formulate orthonormal base to z-axis
onb::onb(const vec3 &n)
{
    axis[2] = unit_vector(n);
    vec3 a = (std::fabsf(n.x()) > 0.9f) ? vec3(0, 1, 0) : vec3(1, 0, 0);
    axis[1] = unit_vector(cross(axis[2], a));
    axis[0] = cross(axis[2], axis[1]);
}
const vec3 &onb::u() const
{
    return onb::axis[0];
}
const vec3 &onb::v() const
{
    return onb::axis[1];
}
const vec3 &onb::w() const
{
    return onb::axis[2];
}
vec3 onb::transform(const vec3 &v) const 
{
    return (v[0] * axis[0]) + (v[1] * axis[1]) + (v[2] * axis[2]);
}