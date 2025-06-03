#pragma once
#include "color.h"
#include "rtw_stb_image.h"
#include "perlin.h"

class texture 
{
    public:
        virtual ~texture() = default;
        virtual color value(float u, float v, const point3& p) const = 0;
};
class solid_color : public texture
{
    private:
        color albedo;

    public:
        solid_color(const color& albedo);
        solid_color(float red, float green, float blue);
        color value(float u, float v, const point3& p) const override;
};
class checker_texture : public texture 
{
    private:
        float inv_scale;
        std::shared_ptr<texture> even;
        std::shared_ptr<texture> odd;
    public:
        checker_texture(float scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd);
        checker_texture(float scale, const color& c1, const color& c2);
        color value(float u, float v, const point3& p) const override;
};

class image_texture : public texture {
    private:
      rtw_image image;
    public:
      image_texture(const char* filename);
  
      color value(float u, float v, const point3& p) const override;
};

class perlin_texture : public texture 
{
    private:
        perlin noise;
        float scale;
    public:
        perlin_texture(float scale);
        color value(float u, float v, const point3& p) const override;
};


