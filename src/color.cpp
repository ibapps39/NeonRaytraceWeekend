#include "color.h"

float linear_to_gamma(float linear_component)
{
    if (linear_component > 0)
    {
        return std::sqrtf(linear_component);
    }
    return 0;
}

color random_albedo() {
    return color(rand_f(), rand_f(), rand_f());
}

color random_albedo(float min, float max)
{
    return color(rand_f(min, max), rand_f(min, max), rand_f(min, max));
}

//Convert vector colors from [0,1] to [0,255], write
void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.00f, 0.999f);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
