#pragma once
#include "common.h"
#include <cassert>

/// @brief 
/// @param arr 
/// @return 
float* flatten_vec3(const std::vector<vec3>& arr)
{
    float* flat_arr = new float[arr.size() * 3];  // Allocate dynamic memory
    for (int i = 0; i < arr.size(); ++i)
    {
        flat_arr[i*3]       = arr[i].x();
        flat_arr[i*3 + 1]   = arr[i].y();
        flat_arr[i*3 + 2]   = arr[i].z();
    }
    return flat_arr;
}
/// @brief 
/// @param arr 
/// @return 
std::vector<float> flatten_vec3_vec(const std::vector<vec3>& arr) {
    std::vector<float> flat;
    flat.reserve(arr.size() * 3);
    for (const auto& v : arr) {
        flat.push_back(v.x());
        flat.push_back(v.y());
        flat.push_back(v.z());
    }
    return flat;
}
/// @brief 
/// @param dst 
/// @param src1 
/// @param src2 
/// @param count 
inline void add_float_c(std::vector<vec3> dst, std::vector<vec3> src1, std::vector<vec3> src2, int count)

{
     for (size_t i = 0; i < count; i++)
     {
        dst[i] = src1[i] + src2[i];
     }
}
/// @brief 
void example_SIMD()
{
    float a[4] = {rand_vecf().x(), rand_vecf().y(), rand_vecf().z(), 1.0f};
    float b[4] = {rand_vecf().x(), rand_vecf().y(), rand_vecf().z(), 1.0f};
    float result[4];

    float32x4_t vec_a = vld1q_f32(a);
    float32x4_t vec_b = vld1q_f32(b);

    float32x4_t vec_result = vaddq_f32(vec_a, vec_b);
    vst1q_f32(result, vec_result);

    for (auto& e : result)
    {
        std::clog << " " << e; 
    }
}
/// @brief AI, SIMD to calculate vector vec3 
/// @param A Vector
/// @param B Vector
/// @param C Sum Vector
void add_vec3_arrays_neon(const std::vector<vec3>& A, const std::vector<vec3>& B, std::vector<vec3>& C) {
    // Ensure all vectors have the same size
    assert(A.size() == B.size());
    C.resize(A.size());
    
    const size_t size = A.size();
    const float* a_ptr = reinterpret_cast<const float*>(A.data());
    const float* b_ptr = reinterpret_cast<const float*>(B.data());
    float* c_ptr = reinterpret_cast<float*>(C.data());
    
    // Process 4 floats at a time (which covers 1.33 vec3s)
    // We need to process 3*size floats total
    const size_t total_floats = size * 3;
    const size_t neon_iterations = total_floats / 4;
    const size_t remaining_floats = total_floats % 4;
    
    // NEON processing - 4 floats at a time
    for (size_t i = 0; i < neon_iterations; ++i) {
        const size_t offset = i * 4;
        
        // Load 4 floats from A and B
        float32x4_t va = vld1q_f32(a_ptr + offset);
        float32x4_t vb = vld1q_f32(b_ptr + offset);
        
        // Add vectors
        float32x4_t vc = vaddq_f32(va, vb);
        
        // Store result
        vst1q_f32(c_ptr + offset, vc);
    }
    
    // Handle remaining floats (0-3 remaining)
    const size_t processed_floats = neon_iterations * 4;
    for (size_t i = 0; i < remaining_floats; ++i) {
        c_ptr[processed_floats + i] = a_ptr[processed_floats + i] + b_ptr[processed_floats + i];
    }
}

/// @brief Alternative version that processes exactly vec3 boundaries
/// @param A 
/// @param B 
/// @param C 
void add_vec3_arrays_neon_aligned(const std::vector<vec3>& A, const std::vector<vec3>& B, std::vector<vec3>& C) {
    assert(A.size() == B.size());
    C.resize(A.size());
    
    const size_t size = A.size();
    
    // Process pairs of vec3s (6 floats = 1.5 NEON registers)
    const size_t pairs = size / 2;
    const size_t remaining = size % 2;
    
    const float* a_ptr = reinterpret_cast<const float*>(A.data());
    const float* b_ptr = reinterpret_cast<const float*>(B.data());
    float* c_ptr = reinterpret_cast<float*>(C.data());
    
    // Process pairs of vec3s (6 floats each iteration)
    for (size_t i = 0; i < pairs; ++i) {
        const size_t offset = i * 6;
        
        // Load first 4 floats
        float32x4_t va1 = vld1q_f32(a_ptr + offset);
        float32x4_t vb1 = vld1q_f32(b_ptr + offset);
        float32x4_t vc1 = vaddq_f32(va1, vb1);
        vst1q_f32(c_ptr + offset, vc1);
        
        // Load remaining 2 floats (using 2-element load)
        float32x2_t va2 = vld1_f32(a_ptr + offset + 4);
        float32x2_t vb2 = vld1_f32(b_ptr + offset + 4);
        float32x2_t vc2 = vadd_f32(va2, vb2);
        vst1_f32(c_ptr + offset + 4, vc2);
    }
    
    // Handle remaining single vec3 if odd number
    if (remaining > 0) {
        const size_t offset = pairs * 6;
        
        // Process last vec3 with scalar operations
        for (int j = 0; j < 3; ++j) {
            c_ptr[offset + j] = a_ptr[offset + j] + b_ptr[offset + j];
        }
    }
}
// Return-based version - returns sum vector
std::vector<vec3> add_vec3_arrays_neon_return(const std::vector<vec3>& A, const std::vector<vec3>& B) {
    assert(A.size() == B.size());
    
    std::vector<vec3> C(A.size());
    
    const size_t size = A.size();
    const float* a_ptr = reinterpret_cast<const float*>(A.data());
    const float* b_ptr = reinterpret_cast<const float*>(B.data());
    float* c_ptr = reinterpret_cast<float*>(C.data());
    
    // Process 4 floats at a time
    const size_t total_floats = size * 3;
    const size_t neon_iterations = total_floats / 4;
    const size_t remaining_floats = total_floats % 4;
    
    // NEON processing - 4 floats at a time
    for (size_t i = 0; i < neon_iterations; ++i) {
        const size_t offset = i * 4;
        
        // Load 4 floats from A and B
        float32x4_t va = vld1q_f32(a_ptr + offset);
        float32x4_t vb = vld1q_f32(b_ptr + offset);
        
        // Add vectors
        float32x4_t vc = vaddq_f32(va, vb);
        
        // Store result
        vst1q_f32(c_ptr + offset, vc);
    }
    
    // Handle remaining floats (0-3 remaining)
    const size_t processed_floats = neon_iterations * 4;
    for (size_t i = 0; i < remaining_floats; ++i) {
        c_ptr[processed_floats + i] = a_ptr[processed_floats + i] + b_ptr[processed_floats + i];
    }
    
    return C;
}

// Alternative return-based version with vec3 boundary alignment
std::vector<vec3> add_vec3_arrays_neon_return_aligned(const std::vector<vec3>& A, const std::vector<vec3>& B) {
    assert(A.size() == B.size());
    
    std::vector<vec3> C(A.size());
    
    const size_t size = A.size();
    const size_t pairs = size / 2;
    const size_t remaining = size % 2;
    
    const float* a_ptr = reinterpret_cast<const float*>(A.data());
    const float* b_ptr = reinterpret_cast<const float*>(B.data());
    float* c_ptr = reinterpret_cast<float*>(C.data());
    
    // Process pairs of vec3s (6 floats each iteration)
    for (size_t i = 0; i < pairs; ++i) {
        const size_t offset = i * 6;
        
        // Load first 4 floats
        float32x4_t va1 = vld1q_f32(a_ptr + offset);
        float32x4_t vb1 = vld1q_f32(b_ptr + offset);
        float32x4_t vc1 = vaddq_f32(va1, vb1);
        vst1q_f32(c_ptr + offset, vc1);
        
        // Load remaining 2 floats
        float32x2_t va2 = vld1_f32(a_ptr + offset + 4);
        float32x2_t vb2 = vld1_f32(b_ptr + offset + 4);
        float32x2_t vc2 = vadd_f32(va2, vb2);
        vst1_f32(c_ptr + offset + 4, vc2);
    }
    
    // Handle remaining single vec3 if odd number
    if (remaining > 0) {
        const size_t offset = pairs * 6;
        
        for (int j = 0; j < 3; ++j) {
            c_ptr[offset + j] = a_ptr[offset + j] + b_ptr[offset + j];
        }
    }
    
    return C;
}



void neon_setup()
{
    srand(time(NULL));
    const int N = 1000000;

    std::vector<vec3> A(N);
    std::vector<vec3> B(N);
    std::vector<vec3> C(N);
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::clog << "\n//////////////////////////////////////////////////\n";
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const char *command = "sysctl -n hw.ncpu";
    std::string max_t = exec(command);
    int max_threads = std::stoi(max_t);
    std::clog << "MAX THREADS per sysctl -n hw.ncpu: maxt = " << max_threads << std::endl;
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double a_time = timeFunction(async_populate, A, 0, N - 1);
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double b_time = timeFunction(start_threads, N, max_threads, B);
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    read_arr(A, B, C);
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::clog << "A - linear, for loop time: " << a_time << "s" << std::endl;
    std::clog << "B - threads, time: " << b_time << "s" << std::endl;
    std::clog << std::endl;
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::clog << "Example NEON SIMD" << std::endl;
    example_SIMD();
    std::clog << std::endl;
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::clog << "NEON: C vs NEON" << std::endl;
    double c_time_c = timeFunction(add_float_c, C, A, B, N);
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    double c_time_neon1 = timeFunction(add_vec3_arrays_neon, A, B, C);
    std::clog << std::endl;
    for (int i = 0; i < 5; i++)
    {
        std::clog << "A" << i << "[" << A[i].x() << ", " << A[i].y() << ", " << A[i].z() << "]\n";
        std::clog << "B" << i << "[" << B[i].x() << ", " << B[i].y() << ", " << B[i].z() << "]\n";
        std::clog << "C" << i << "[" << C[i].x() << ", " << C[i].y() << ", " << C[i].z() << "]\n";
        std::clog << "\n";
    }
    std::clog << "add_float_c (C = A+B) - linear, for loop time: " << c_time_c << "s" << std::endl;
    std::clog << "add_float_neon1 (C = A+B) - neon, time: " << c_time_neon1 << "s" << std::endl;
    std::clog << std::endl;
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::clog << "\n//////////////////////////////////////////////////\n";

}