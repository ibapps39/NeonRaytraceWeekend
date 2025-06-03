#pragma once
#include "hittable.h"
#include "texture.h"
#include "onb.h"
class material
{
public:
    virtual ~material() = default;
    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered, float &pdf) const
    {
        return false;
    }
    virtual color emitted(float u, float v, const point3 &p) const
    {
        return color(0, 0, 0);
    };
    virtual float scatter_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const
    {
        return 0.0f;
    }
};

class lambertian : public material
{
private:
    color albedo;
    std::shared_ptr<texture> tex;

public:
    lambertian(const color &albedo);
    lambertian(std::shared_ptr<texture> tex);
    bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered, float &pdf) const override;
    float scatter_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const override;
};

class metal : public material
{
private:
    color albedo;
    float fuzz;

public:
    metal(const color &albedo, float fuzz);
    bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered, float &pdf) const override;
};

class dielectric : public material
{
private:
    float refract_index;
    static float reflectance(float cosine, float refraction_index);

public:
    dielectric(float refract_index);
    bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered, float &pdf) const override;
};
class diffuse_light : public material
{
private:
    std::shared_ptr<texture> tex;

public:
    diffuse_light(std::shared_ptr<texture> tex);
    diffuse_light(const color &emit);
    color emitted(float u, float v, const point3 &p) const override;
};

class isotropic : public material
{
private:
    std::shared_ptr<texture> tex;

public:
    isotropic(const color &albedo);
    isotropic(std::shared_ptr<texture> tex);
    bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered, float &pdf) const override;
    float scatter_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const override;
};