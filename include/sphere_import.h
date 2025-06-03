#pragma once
#include "rt_common.h"

#include <iostream>
#include <iomanip>

inline float func_sub(const vec3& d) {
    float cosine_squared = (float)d.z()*d.z();
    return cosine_squared;
}

inline float pdf_sphere(const vec3& d) {
    return 1 / (4*pi);
}

void sphere
() {
    int N = 1000000;
    float sum = 0.0f;
    for (int i = 0; i < N; i++) {
        vec3 d = rand_unit_vec();
        float f_d = func_sub(d);
        sum += f_d / pdf_sphere(d);
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "I = " << sum / N << '\n';
}