#include "vec3.h"
#include "rt_common.h"
// Constructors
vec3::vec3() : e{0, 0, 0} {}
vec3::vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}

// Accessors
float vec3::x() const { return e[0]; }
float vec3::y() const { return e[1]; }
float vec3::z() const { return e[2]; }

// Negation
vec3 vec3::operator-() const {
    return vec3(-e[0], -e[1], -e[2]);
}

// Indexing
float vec3::operator[](int i) const { return e[i]; }
float& vec3::operator[](int i) { return e[i]; }

// Compound assignment
vec3& vec3::operator+=(const vec3& v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

vec3& vec3::operator*=(float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

vec3& vec3::operator/=(float t) {
    return *this *= 1.0f / t;
}

float vec3::length_squared() const {
    return ( (e[0]*e[0]) + (e[1]*e[1]) + (e[2]*e[2]) );
}

// Length
float vec3::length() const {
    return std::sqrt(length_squared());
}

// Utility functions
std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

 vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

 vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

 vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

 vec3 operator*(float t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

 vec3 operator*(const vec3& v, float t) {
    return t * v;
}

 vec3 operator/(const vec3& v, float t) {
    return (1.0f / t) * v;
}

 float dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

 vec3 cross(const vec3& u, const vec3& v) {
    return vec3(
        u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]
    );
}
vec3 vec3::rand_reflect()
{
    return vec3(rand_f(), rand_f(), rand_f());
}
vec3 vec3::rand_reflect(float min, float max)
{
    return vec3(rand_f(min, max), rand_f(min, max), rand_f(min, max));
}
// NormalIze
 vec3 unit_vector(const vec3& v) {
    return v / v.length();
}
 vec3 rand_unit_vec()
{
    while (true)
    {
        vec3 v = vec3::rand_reflect(-1, 1);
        float len_sq = v.length_squared();
        if ( (__FLT_MIN__ < len_sq) && (len_sq <= 1.0f))
        {
            return v/sqrtf(len_sq);
        }
    }

}
 vec3 random_on_hemisphere(const vec3& normal)
{
    vec3 on_unit_sphere = rand_unit_vec();
    if (dot(normal, on_unit_sphere) > 0.0)
    {
        return on_unit_sphere;
    } else {
        return -on_unit_sphere;
    }
}

bool vec3::near_zero()
{
    float min_f = 1e-8;
    return  (std::fabs(e[0]) < min_f) && (std::fabs(e[1]) < min_f) && (std::fabs(e[2]) < min_f);
    
}

vec3 reflect(const vec3& v, const vec3& n)
{
    return v - (2*dot(v,n)*n);
}
vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat)
{
    float cos_theta = std::fmin(dot(-uv, n), 1.0f);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -std::sqrtf(std::fabsf(1.0f - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
vec3 random_in_unit_disk()
{
    while (true)
    {
        vec3 p = vec3(rand_f(-1, 1), rand_f(-1, 1), 0);
        if (p.length_squared() < 1)
        {
            return p;
        }
    }
}
vec3 vec3::random(float min, float max)
{
    return vec3(rand_f(min,max), rand_f(min,max), rand_f(min,max));
}
vec3 vec3::random()
{
    return vec3(rand_f(), rand_f(), rand_f());
}

vec3 random_cosf_dir()
{
    float r1 = rand_f();
    float r2 = rand_f();
    float phi = 2.0f*pi*r1;
    float x = std::cosf(phi)*std::sqrtf(r2);
    float y = std::sinf(phi)*std::sqrtf(r2);
    float z = std::sqrtf(1.0f-r2);

    return vec3(x, y, z);
}