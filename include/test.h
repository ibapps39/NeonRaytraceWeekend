#pragma once
#include "common.h"
#include <cassert>
using namespace std;
void test_simd(vector<vec3>& a, vector<vec3>& b)
{
    assert(a.size() == b.size());
    vector<vec3> c;
    c.reserve(a.size());
    const size_t size = a.size();
    const size_t float_count = size / 4;
    const size_t floats_remained = size % 4;
}