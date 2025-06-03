#pragma once
#include <random>
/// @brief 
class vec3 
{
    private:
    public:
        /// @brief 
        float e[3];
        /// @brief 
        enum dim {X, Y, Z};
        /// @brief 
        vec3() : e{0,0,0} {}
        /// @brief 
        /// @param x 
        /// @param y 
        /// @param z 
        vec3(float x, float y, float z) : e{x, y, z} {}
        float x() const { return e[X]; }
        float y() const { return e[Y]; };
        float z() const { return e[Z]; };
        /// @brief 
        /// @param v 
        /// @return 
        vec3& operator+ (const vec3& v) {
            e[X] += v.e[X];
            e[Y] += v.e[Y];
            e[Z] += v.e[Z];
            return *this;
        }
        /// @brief 
        /// @param v 
        /// @return 
        vec3& operator+= (const vec3& v) {
            e[X] += v.e[X];
            e[Y] += v.e[Y];
            e[Z] += v.e[Z];
            return *this;
        }
        /// @brief 
        /// @param v 
        /// @return 
        vec3& operator- (const vec3& v) {
            e[X] -= v.e[X];
            e[Y] -= v.e[Y];
            e[Z] -= v.e[Z];
            return *this;
        }
        /// @brief 
        /// @param v 
        /// @return 
        vec3& operator-= (const vec3& v) {
            e[X] -= v.e[X];
            e[Y] -= v.e[Y];
            e[Z] -= v.e[Z];
            return *this;
        }
        /// @brief 
        /// @param S 
        /// @return 
        vec3& operator*= (float S) {
            e[X] *= S;
            e[Y] *= S;
            e[Z] *= S;
            return *this;
        }
        /// @brief 
        /// @param S 
        /// @return 
        vec3& operator/= (float S) {
            return *this *= 1/S;
        }
        /// @brief 
        /// @return 
        float len_sqf() const {
            return ( (e[0]*e[0]) + (e[1]*e[1]) + (e[2]*e[2]) );
        }
        /// @brief 
        /// @return 
        float len_f() const {
            return sqrtf(vec3::len_sqf());
        }
        /// @brief 
        /// @return 
        float len_fast() const {
            // FUTURE FAST APPROX
            int x = vec3::len_f();
            int i = *(int*)&x;
            i = (i >> 1) + 0x1FC00000;  // initial guess
            return *(float*)&i;
        }
        
};
/// @brief 
/// @return 
inline vec3 rand_vec() {
    return vec3( 
        rand()%101 , 
        rand()%101 , 
        rand()%101 
    );
}
/// @brief 
/// @return 
inline vec3 rand_vecf() {
    return vec3( 
        rand()%101 + (1/(rand()%100)) , 
        rand()%101 + (1/(rand()%100)) , 
        rand()%101 + (1/(rand()%100)) 
    );
}

