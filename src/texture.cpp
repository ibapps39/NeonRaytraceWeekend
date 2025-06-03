#include "texture.h"

solid_color::solid_color(const color& albedo) : albedo(albedo)
{}
solid_color::solid_color(float red, float green, float blue) : solid_color(color(red, green, blue))
{}
color solid_color::value(float u, float v, const point3& p) const
{
    return albedo;
}

checker_texture::checker_texture(float scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
    : inv_scale(1.0f/scale), even(even), odd(odd)
{}
checker_texture::checker_texture(float scale, const color& c1, const color& c2) 
    : checker_texture(
        scale, 
        std::make_shared<solid_color>(c1),
        std::make_shared<solid_color>(c2)
    )
{}
color checker_texture::value(float u, float v, const point3& p) const
{
    int xInt = int( std::floorf( p.x() * inv_scale ) );
    int yInt = int( std::floorf( p.y() * inv_scale ) );
    int zInt = int( std::floorf( p.z() * inv_scale ) );

    bool isEven = (xInt + yInt + zInt) % 2 == 0;

    return isEven ? even->value(u, v, p) : odd->value(u, v, p);
}

image_texture::image_texture(const char* filename) : image(filename)
{}
color image_texture::value(float u, float v, const point3& p) const 
{
            // If we have no texture data, then return solid cyan as a debugging aid.
            if (image.height() <= 0) return color(0,1,1);

            // Clamp input texture coordinates to [0,1] x [1,0]
            u = interval(0,1).clamp(u);
            v = 1.0 - interval(0,1).clamp(v);  // Flip V to image coordinates
    
            auto i = int(u * image.width());
            auto j = int(v * image.height());
            auto pixel = image.pixel_data(i,j);
    
            auto color_scale = 1.0 / 255.0;
            return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
}
perlin_texture::perlin_texture(float scale) : scale(scale)
{}
color perlin_texture::value(float u, float v, const point3& p) const
{
    //std::clog << "noise.noise(p): " << noise.noise(p) << std::endl;
    //std::clog << "color(1,1,1) * noise.noise(p)" << color(1,1,1) * noise.noise(p) << std::endl;
    return color(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));

}