#pragma once
#include <thread>
#include <vector>
#include <memory>
#include <random>
#include <time.h>
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <arm_neon.h>
#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"
#include <iostream>
#include <iomanip>
#include "integrate.h"
#include "estimate.h"

#include "vec3.h"
#include "neon_op.h"


/// @brief 
/// @tparam Func 
/// @tparam ...Args 
/// @param f 
/// @param ...args 
/// @return 
template<typename Func, typename... Args>
auto timeFunction(Func&& f, Args&&... args) {
    auto begin = std::chrono::steady_clock::now();
    if constexpr (std::is_void_v<decltype(f(args...))>) {
        f(std::forward<Args>(args)...);
        auto end = std::chrono::steady_clock::now();
        return (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0;
    } else {
        auto result = f(std::forward<Args>(args)...);
        auto end = std::chrono::steady_clock::now();
        return std::make_pair(result, (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0);
    }
}
/// @brief 
/// @param A 
/// @param B 
/// @param C 
void read_arr(const std::vector<vec3> &A, const std::vector<vec3> &B, const std::vector<vec3> &C)
{

    std::cout << "First 10 elements: A" << std::endl;
    for (size_t i = 0; i < 11; i++)
    {
        std::cout << "A" << i << "[" << A[i].x() << ", " << A[i].y() << ", " << A[i].z() << "]\n";
    }
    std::cout << std::endl;
    std::cout << "First 10 elements: B" << std::endl;
    for (size_t i = 0; i < 11; i++)
    {
        std::cout << "B" << i << "[" << B[i].x() << ", " << B[i].y() << ", " << B[i].z() << "]\n";
    }
    std::cout << std::endl;
    if (C[0].x() == 0 && C[0].x() == 0 && C[0].x() == 0) {return;}

        std::cout << "First 10 elements: B" << std::endl;
        for (size_t i = 0; i < 11; i++)
        {
            std::cout << "C" << i << "[" << C[i].x() << ", " << C[i].y() << ", " << C[i].z() << "]\n";
        }
    std::cout << std::endl;
}

/// @brief async, give an array random floats
/// @param v 
/// @param start 
/// @param end 
void async_populate(std::vector<vec3> &v, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        v[i] = rand_vecf();
    }
}

/// @brief threads to vector that are prepopulated with tiles
/// @param threads_v 
/// @param start 
/// @param end 
/// @param v 
void push_threads(std::vector<std::thread> &threads_v, int start, int end, std::vector<vec3> &v)
{
    threads_v.push_back(std::thread(&async_populate, std::ref(v), start, end));
}
/// @brief 
/// @param N 
/// @param max_threads 
/// @param arr 
void start_threads(int N, int max_threads, std::vector<vec3>& arr)
{
        std::vector<std::thread> threads;
        int chunk_size = N / max_threads; // 1000000 / 8 = 125000
        for (int i = 0; i < max_threads; i++)
        {
            int start = i * chunk_size;
            int end = (i == max_threads - 1) ? N : (i + 1) * chunk_size;
            push_threads(threads, start, end, arr);
        }

        for (auto &t : threads)
        {
            t.join();
        }
}
/// @brief 
/// @param cmd 
/// @return 
inline std::string exec(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;

    // Open a pipe to the command
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    // Read the output into the result string
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return result;
}