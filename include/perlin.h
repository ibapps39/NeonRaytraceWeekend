#pragma once
#include "rt_common.h"
class perlin
{
    private:
        static const int point_count = 256;
        vec3 randvec[point_count];
        int perm_x[point_count];
        int perm_y[point_count];
        int perm_z[point_count];

        static void permute(int* p, int n);
        static void perlin_generate_perm(int* p);
        static float trilinear_interp(float c[2][2][2], float u, float v, float w);
        static float perlin_interp(const vec3 c[2][2][2], float u, float v, float w);
        
        
    public:
        perlin();
        float noise(const point3& p) const;
        float turb(const point3& p, int depth) const;

};
