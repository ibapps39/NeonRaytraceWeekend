#include "camera.h"

void camera::initialize()
{
    // Compute the image height from the width and aspect ratio
    image_height = int(image_width / aspect_ratio);
    // Ensure the image height is at least 1 to prevent invalid dimensions
    image_height = (image_height < 1) ? 1 : image_height;

    // Determine the square root of samples per pixel for stratified sampling
    sqrt_spp = int(std::sqrtf(samples_per_px));
    // Calculate the scaling factor for pixel colors based on total samples
    px_sample_scale = float(1.0f / (sqrt_spp * sqrt_spp));
    // Precompute the reciprocal of sqrt_spp for sampling calculations
    recip_sqrt_spp = float(1.0f / sqrt_spp);

    // Set the camera's position to the specified 'lookfrom' point
    camera_center = lookfrom;

    // Define the camera's orientation axes
    // Z-axis points from the camera position to the look-at point
    cam_z = unit_vector(lookfrom - lookat);
    // X-axis is perpendicular to Z-axis and the up vector
    cam_x = unit_vector(cross(camera_up, cam_z));
    // Y-axis completes the orthonormal basis (already normalized)
    cam_y = cross(cam_z, cam_x);

    // Calculate viewport size based on field of view and focus distance
    camera_theta = (float)degrees_to_radians(vfov);                         // Convert vertical FOV to radians
    camera_height = std::tanf(camera_theta / 2.0f);                         // Half-height of the viewport
    viewport_height = 2.0f * camera_height * focus_dist;                    // Full viewport height
    viewport_width = viewport_height * (float(image_width) / image_height); // Width based on aspect ratio

    // Compute vectors spanning the viewport's width and height
    viewport_x = viewport_width * cam_x;   // Horizontal edge vector
    viewport_y = viewport_height * -cam_y; // Vertical edge vector (downward)

    // Determine per-pixel step sizes across the viewport
    pixel_delta_x = viewport_x / image_width;  // Horizontal pixel step
    pixel_delta_y = viewport_y / image_height; // Vertical pixel step

    // Locate the top-left pixel in 3D space
    point3 viewport_upper_left = camera_center - (focus_dist * cam_z) - viewport_x / 2.0f - viewport_y / 2.0f;
    pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_x + pixel_delta_y); // Center of the first pixel

    // Set up defocus disk for depth of field effect
    float defocus_radius = focus_dist * std::tanf(degrees_to_radians(defocus_angle / 2.0f));
    def_disk_xr = cam_x * defocus_radius; // X-basis for defocus disk
    def_disk_yr = cam_y * defocus_radius; // Y-basis for defocus disk
}

color camera::ray_color(const ray &r, int depth, const hittable &world) const
{
    // Stop recursion if depth limit is reached, returning black
    if (depth <= 0)
    {
        return color(0, 0, 0);
    }

    hit_record rec;
    // Check for ray-object intersection; return background color if no hit
    if (!world.hit(r, interval(0.001, infinity), rec))
    {
        return background;
    }

    ray scattered;
    color attenuation;
    float pdf_val;
    // Get light emitted by the material at the hit point
    color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

    // If the material doesn't scatter, return only its emitted color
    if (!rec.mat->scatter(r, rec, attenuation, scattered, pdf_val))
    {
        return color_from_emission;
    }
    float scattering_pdf = rec.mat->scatter_pdf(r, rec, scattered);
    pdf_val = scattering_pdf;
    // Recursively compute scattered color and combine with emission
    color color_from_scatter = (attenuation * scattering_pdf * ray_color(scattered, depth-1, world)) / pdf_val;

    return color_from_emission + color_from_scatter;
}

// Generate a random offset within a unit square for sampling
vec3 camera::sample_square() const
{
    return vec3(rand_f() - 0.5f, rand_f() - 0.5f, 0); // Range: [-.5, -.5] to [.5, .5]
}

// Sample a point on the defocus disk for depth of field
point3 camera::defocus_sampling() const
{
    vec3 p = random_in_unit_disk();                                     // Random point in unit disk
    return camera_center + (p[0] * def_disk_xr) + (p[1] * def_disk_yr); // Map to defocus disk
}

// Perform stratified sampling within a pixel's sub-grid
vec3 camera::sample_square_stratified(int s_i, int s_j) const
{
    // Compute random position within the (s_i, s_j) sub-pixel grid
    float px = float((s_i + rand_f()) * recip_sqrt_spp) - 0.5f;
    float py = float((s_j + rand_f()) * recip_sqrt_spp) - 0.5f;
    return vec3(px, py, 0); // Offset within [-.5, -.5] to [.5, .5]
}

// Create a ray for a pixel with stratified sampling
ray camera::get_ray(int i, int j, int s_i, int s_j) const
{
    vec3 offset = sample_square_stratified(s_i, s_j); // Sub-pixel offset
    // Compute the sample point in viewport space
    vec3 px_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_x) + ((j + offset.y()) * pixel_delta_y);
    // Use camera center or defocus disk for ray origin
    vec3 ray_origin = (defocus_angle <= 0) ? camera_center : defocus_sampling();
    vec3 ray_direction = px_sample - ray_origin; // Direction to sample point
    float ray_time = rand_f();                   // Random time for motion blur
    return ray(ray_origin, ray_direction, ray_time);
}

// Render the scene into a PPM image
void camera::render(const hittable &world)
{
    initialize(); // Set up camera parameters

    // Write PPM header: format, width, height, max color value
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    // Loop over each pixel in the image
    for (int j = 0; j < image_height; j++)
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush; // Progress update
        for (int i = 0; i < image_width; i++)
        {
            color pixel_color(0, 0, 0); // Initialize pixel color
            // Stratified sampling within the pixel
            for (int s_j = 0; s_j < sqrt_spp; s_j++)
            {
                for (int s_i = 0; s_i < sqrt_spp; s_i++)
                {
                    ray r = get_ray(i, j, s_i, s_j);               // Generate ray for this sample
                    pixel_color += ray_color(r, max_depth, world); // Add ray's color contribution
                }
            }
            // Average the color over all samples and output
            write_color(std::cout, px_sample_scale * pixel_color);
        }
    }

    std::clog << "\rDone.                 \n"; // Signal completion
}