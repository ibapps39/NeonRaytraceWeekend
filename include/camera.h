#pragma once
#include "hittable.h"
#include "material.h"

// Needs to be set for calculating height. Currently 800.0f
#define DEFAULT_VIEW_WIDTH (800.0f)
// 1.77777777778f
#define HARDCODED_ASPECT_RATIO (16.0f / 9.0f)
// DEFAULT_VIEW_WIDTH / (HARDCODED_ASPECT_RATIO). Easier to maintain desired aspect ration if calculated
#define DEFAULT_VIEW_HEIGHT ( (DEFAULT_VIEW_WIDTH) / (HARDCODED_ASPECT_RATIO) )
// 1.77777777778f
#define HARDCODED_ASPECT_RATIO_VALUE 1.77777777778f
// 16:9 ~= 1.77..8
#define DEFAULT_ASPECT_RATIO ( (DEFAULT_VIEW_WIDTH) / (DEFAULT_VIEW_HEIGHT) )

#define MAX_P3_COLOR_SIZE 255

class camera
{
private:
    // Calculate the image height, and ensure that it's at least 1.
    int image_height;
    int    sqrt_spp;             // Square root of number of samples per pixel
    float recip_sqrt_spp;       // 1 / sqrt_spp
    // Viewport widths less than one are ok since they are real valued.
    float viewport_height;
    float viewport_width;
    vec3 viewport_x;
    vec3 viewport_y;
    vec3 viewport_upper_left;
    //
    float focal_length;
    point3 camera_center;
    vec3 pixel_delta_x;
    vec3 pixel_delta_y;
    point3 pixel00_loc;

    float px_sample_scale; // Color scale factor for a sum of pixel samples

    float camera_theta;
    float camera_height;
    vec3 cam_x, cam_y, cam_z;
    
    vec3 def_disk_xr; // Defocus disk horizontal radius
    vec3 def_disk_yr; // Defocus disk vertical radius

    void initialize();
    color ray_color(const ray &r, int depth, const hittable &world) const;
    vec3 sample_square() const;
    ray get_ray(int i, int j, int s_i, int s_j) const;
    point3 defocus_sampling() const;
    vec3 sample_square_stratified(int s_i, int s_j) const;


public:
    float aspect_ratio;
    int image_width;
    int samples_per_px;
    int max_depth = 10;
    float vfov;
    point3 lookat;
    point3 lookfrom;
    vec3 camera_up;
    float defocus_angle;  // Variation angle of rays through each pixel
    float focus_dist;    // Distance from camera lookfrom point to plane of perfect focus
    color background;    
    void render(const hittable &world);
};