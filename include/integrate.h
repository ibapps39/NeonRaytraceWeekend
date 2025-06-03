#pragma once
#include "rt_common.h"
#include "iostream"
#include "iomanip"

float icd(float d) {
    return 8.0f * std::powf(d, 1.0f/3.0f);
}

float pdf(float x) {
    return (3.0f/8.0f) * float(x*x);;
}

inline void integrate_monty_avg()
{
    int N = 100000;
    float sum = 0.0f;

    for (int i = 0; i < N; i++)
    {
        float x = icd(rand_f());
        sum += x*x;
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "I = " << (sum/N) << std::endl;
}