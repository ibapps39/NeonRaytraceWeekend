#include "sphere.h"

sphere::sphere(const point3 &static_center, float radius, std::shared_ptr<material> mat) : 
displacement_ray(static_center, vec3(0,0,0)), radius(std::fmax(0, radius)), mat(mat)
{
    vec3 rvec = vec3(radius, radius, radius);
    bbox = aabb(static_center - rvec, static_center + rvec);
}

sphere::sphere(const point3 &center1, const point3 &center2, float radius, std::shared_ptr<material> mat) : 
displacement_ray(center1, center2-center1), radius(std::fmax(0, radius)), mat(mat)
{
    vec3 rvec = vec3(radius, radius, radius);
    aabb initial_box(displacement_ray.at(0) - rvec, displacement_ray.at(0) + rvec);
    aabb dt_box(displacement_ray.at(1) - rvec, displacement_ray.at(1) + rvec);
    bbox = aabb(initial_box, dt_box);
}
void sphere::get_sphere_uv(const point3& p, float& u, float& v)
{
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
    float phi = std::atan2f(-p.z(), p.x()) + pi;
    float theta = std::acosf(-p.y());

    //normalize 
    u = phi/(2.0f*pi);
    v = (theta/float(pi));
}

// returns this->bbox.
aabb sphere::bounding_box() const { return bbox; } 

bool sphere::hit(const ray& r, interval ray_t, hit_record& hit_rec) const 
{
    point3 current_center = displacement_ray.at(r.time());
    vec3 oc = current_center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;
    float discriminant = h*h - a*c;

    if (discriminant < 0) { return false; }

    float sqrt_det = std::sqrtf(discriminant);
    float root = (h - sqrt_det) / a;
    if (!ray_t.surrounds(root))
    {
        root = (h + sqrt_det)/a;
        if (!ray_t.surrounds(root))
        {
            return false;
        }
    }
    hit_rec.t = root;
    //hit position
    hit_rec.p = r.at(hit_rec.t);
    // Sphere, so can norm. w/radius
    vec3 outward_normal = (hit_rec.p - current_center) / radius;
    // record a hit and by how much, and if a surface. Records surface normals
    hit_rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, hit_rec.u, hit_rec.v);
    hit_rec.mat = mat;
    
    return true;
}
