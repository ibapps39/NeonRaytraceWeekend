#include "rt_common.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>

struct sample
{
    float x;
    float p_x;
};

bool compare_by_x(const sample& a, const sample& b)
{
    return a.x < b.x;
}

void expir_est()
{
    const unsigned int N = 10000;
    sample samples[N];
    float sum = 0.0f;
    // Iterate through all samples
    for (unsigned int i = 0; i < N; i++)
    {
        float uniform_x = rand_f(0, 2 * pi);
        // p(x) = e^(-x/2pi)sin^2(x), du,dx
        float sinx = std::sinf(uniform_x);                           // dx
        float p_x = expf(-1 * uniform_x / (2 * pi)) * (sinx * sinx); // du
        sum += p_x;
        sample current_sample = {uniform_x, p_x};
        samples[i] = current_sample;
    }

    // sort the samples, via x-component, think the bell curve
    std::sort(std::begin(samples), std::end(samples), compare_by_x);

    // Find sample of the halfpoint
    float half_sum = sum / 2.0f;
    float halfway_point = 0.0f;
    float accum = 0.0f;
    for (unsigned int i = 0; i < N; i++)
    {
        accum += samples[i].p_x;
        if (accum >= half_sum)
        {
            halfway_point = samples[i].x;
            break;
        }
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Average = " << sum / N << '\n';
    std::cout << "Area under curve = " << 2 * pi * sum / N << '\n';
    std::cout << "Halfway = " << halfway_point << '\n';
}
