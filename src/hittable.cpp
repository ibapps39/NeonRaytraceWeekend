#include "hittable.h"
#include "rt_common.h"

void hit_record::set_face_normal(const ray &r, const vec3 &outward_normal)
{
  // Sets the hit record normal vector.
  // NOTE: the parameter `outward_normal` is assumed to have unit length.
  front_face = dot(r.direction(), outward_normal) < 0.0f;
  // if >0, its face outside towards camera, otherwise its inside, and facing the same direction
  normal = front_face ? outward_normal : -outward_normal;
};

aabb translate::bounding_box() const
{
  return bbox;
}
translate::translate(std::shared_ptr<hittable> object, const vec3 &offset) : object(object), offset(offset)
{
  bbox = translate::object->bounding_box() + offset;
}
bool translate::hit(const ray &r, interval ray_t, hit_record &rec) const
{
  // Move the ray backwards by the offset
  ray offset_r(r.origin() - offset, r.direction(), r.time());

  // Determine whether an intersection exists along the offset ray (and if so, where)
  if (!object->hit(offset_r, ray_t, rec))
    return false;

  // Move the intersection point forwards by the offset
  rec.p += offset;

  return true;
}


aabb rotate_y::bounding_box() const
{
  return bbox;
}

rotate_y::rotate_y(std::shared_ptr<hittable> object, float angle) : object(object)
{
  float radians = degrees_to_radians(angle);
  sin_theta = std::sin(radians);
  cos_theta = std::cos(radians);
  bbox = object->bounding_box();

  point3 min(infinity, infinity, infinity);
  point3 max(-infinity, -infinity, -infinity);

  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      for (int k = 0; k < 2; k++)
      {
        float x = i * bbox.x.max + (1 - i) * bbox.x.min;
        float y = j * bbox.y.max + (1 - j) * bbox.y.min;
        float z = k * bbox.z.max + (1 - k) * bbox.z.min;

        float newx = cos_theta * x + sin_theta * z;
        float newz = -sin_theta * x + cos_theta * z;

        vec3 tester(newx, y, newz);

        for (int c = 0; c < 3; c++)
        {
          min[c] = std::fmin(min[c], tester[c]);
          max[c] = std::fmax(max[c], tester[c]);
        }
      }
    }
  }

  bbox = aabb(min, max);
}

bool rotate_y::hit(const ray &r, interval ray_t, hit_record &rec) const
{
  // Transform the ray from world space to object space.
  auto origin = point3(
      (cos_theta * r.origin().x()) - (sin_theta * r.origin().z()),
      r.origin().y(),
      (sin_theta * r.origin().x()) + (cos_theta * r.origin().z()));

  auto direction = vec3(
      (cos_theta * r.direction().x()) - (sin_theta * r.direction().z()),
      r.direction().y(),
      (sin_theta * r.direction().x()) + (cos_theta * r.direction().z()));

  ray rotated_r(origin, direction, r.time());

  // Determine whether an intersection exists in object space (and if so, where).

  if (!object->hit(rotated_r, ray_t, rec))
    return false;

  // Transform the intersection from object space back to world space.

  rec.p = point3(
      (cos_theta * rec.p.x()) + (sin_theta * rec.p.z()),
      rec.p.y(),
      (-sin_theta * rec.p.x()) + (cos_theta * rec.p.z()));

  rec.normal = vec3(
      (cos_theta * rec.normal.x()) + (sin_theta * rec.normal.z()),
      rec.normal.y(),
      (-sin_theta * rec.normal.x()) + (cos_theta * rec.normal.z()));

  return true;
}