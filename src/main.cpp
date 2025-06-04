#include "common.h"
int main()
{
        srand(time(NULL));
    const int N = 1000000;

    std::vector<vec3> A(N);
    std::vector<vec3> B(N);
    std::vector<vec3> C(N);
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::clog << "\n//////////////////////////////////////////////////\n";
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const char *command = "sysctl -n hw.ncpu";
    std::string max_t = exec(command);
    int max_threads = std::stoi(max_t);
    std::clog << "MAX THREADS per sysctl -n hw.ncpu: maxt = " << max_threads << std::endl;
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double a_time = timeFunction(async_populate, A, 0, N - 1);
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double b_time = timeFunction(start_threads, N, max_threads, B);
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    read_arr(A, B, C);
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::clog << "A - linear, for loop time: " << a_time << "s" << std::endl;
    std::clog << "B - threads, time: " << b_time << "s" << std::endl;
    std::clog << std::endl;
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::clog << "Example NEON SIMD" << std::endl;
    example_SIMD();
    std::clog << std::endl;
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::clog << "NEON: C vs NEON" << std::endl;
    double c_time_c = timeFunction(add_float_c, C, A, B, N);
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    double c_time_neon1 = timeFunction(add_vec3_arrays_neon, A, B, C);
    std::clog << std::endl;
    for (int i = 0; i < 5; i++)
    {
        std::clog << "A" << i << "[" << A[i].x() << ", " << A[i].y() << ", " << A[i].z() << "]\n";
        std::clog << "B" << i << "[" << B[i].x() << ", " << B[i].y() << ", " << B[i].z() << "]\n";
        std::clog << "C" << i << "[" << C[i].x() << ", " << C[i].y() << ", " << C[i].z() << "]\n";
        std::clog << "\n";
    }
    std::clog << "add_float_c (C = A+B) - linear, for loop time: " << c_time_c << "s" << std::endl;
    std::clog << "add_float_neon1 (C = A+B) - neon, time: " << c_time_neon1 << "s" << std::endl;
    std::clog << std::endl;
    //\\/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::clog << "\n//////////////////////////////////////////////////\n";
    
    hittable_list world;
    auto red = std::make_shared<lambertian>(color(.65, .05, .05));
    auto white = std::make_shared<lambertian>(color(.73, .73, .73));
    auto green = std::make_shared<lambertian>(color(.12, .45, .15));
    auto light = std::make_shared<diffuse_light>(color(15, 15, 15));

    // Cornell box sides
    world.add(std::make_shared<quad>(point3(555, 0, 0), vec3(0, 0, 555), vec3(0, 555, 0), green));
    world.add(std::make_shared<quad>(point3(0, 0, 555), vec3(0, 0, -555), vec3(0, 555, 0), red));
    world.add(std::make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(std::make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 0, -555), white));
    world.add(std::make_shared<quad>(point3(555, 0, 555), vec3(-555, 0, 0), vec3(0, 555, 0), white));

    // Light
    world.add(std::make_shared<quad>(point3(213, 554, 227), vec3(130, 0, 0), vec3(0, 0, 105), light));

    // Box 1
    std::shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(box1, vec3(265, 0, 295));
    world.add(box1);

    // Box 2
    std::shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>(box2, vec3(130, 0, 65));
    world.add(box2);

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 1200;
    cam.samples_per_px = 100;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.camera_up = vec3(0, 1, 0);

    cam.defocus_angle = 0;
    cam.focus_dist = 10.0f;

    cam.render(world);

    return 0;
}
