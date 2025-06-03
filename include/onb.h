#pragma once
#include "rt_common.h"
class onb
{
    private:
        vec3 axis[2];
    public:
        onb(const vec3& n);
        const vec3& u() const;
        const vec3& v() const;
        const vec3& w() const;
        vec3 transform(const vec3& v) const;
};