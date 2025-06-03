#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

// using std::make_shared;
// using std::shared_ptr;

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415927f;

inline float degrees_to_radians(float degrees)
{
    return (degrees*(float)pi) / 180.0f;
}

inline float rand_f()
{
    return float(rand() / (RAND_MAX + 1.0f));
}
inline float rand_f(float min, float max)
{
    return min + (max-min)*rand_f();
}
inline float rand_int(int min, int max)
{
    return int(rand_f(min, max+1));
}

