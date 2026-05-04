#include "../cameras/Perspective.hpp"
#include "../geometry/Triangle.hpp"
#include "../materials/Matte.hpp"
#include "../materials/Phong.hpp"
#include "../materials/Reflective.hpp"
#include "../samplers/Jittered.hpp"
#include "../tracers/Whitted.hpp"
#include "../lights/PointLight.hpp"
#include "../lights/Directional.hpp"
#include "../utilities/Constants.hpp"
#include "../world/World.hpp"
#include "crystalgen.hpp"
#include <iostream>

void World::build(void) {
    srand(42);

    float x0 = -200, x1 = 200;
    float z0 = -350, z1 = 120;
    float fy = 0, cy = 140;

    vplane.top_left.x = -36;
    vplane.top_left.y = 55;
    vplane.top_left.z = 10;
    vplane.bottom_right.x = 36;
    vplane.bottom_right.y = 15;
    vplane.bottom_right.z = 10;
    vplane.hres = 1920;
    vplane.vres = 1080;

    bg_color = RGBColor(0.01f, 0.01f, 0.02f);
    ambient  = RGBColor(0.22f, 0.22f, 0.28f);

    set_camera(new Perspective(0, 70, 110));
    sampler_ptr = new Jittered(camera_ptr, &vplane, 2);
    tracer_ptr  = new Whitted(this, 3);

    Material *fm = new Reflective(0.15f, 0.35f, 0.2f, RGBColor(0.18f, 0.18f, 0.22f), 50, 0.3f);
    Material *wm = new Matte(0.3f, 0.55f, RGBColor(0.28f, 0.24f, 0.2f));

    add_cave_walls(*this, fm, wm, x0, x1, z0, z1, fy, cy);
    populate_crystals(*this, x0, x1, z0, z1, fy, cy);

    std::cout << "total geometry: " << geometry.size() << "\n";
    build_accel();

    // two shadow-casting key lights
    add_light(new PointLight(Point3D(-50, 60, -30),  RGBColor(1.0f, 0.8f, 0.45f), 5.0f));
    add_light(new PointLight(Point3D( 60, 70, -120), RGBColor(0.45f, 0.65f, 1.0f), 4.5f));

    PointLight *p1 = new PointLight(Point3D(20, 35, -220), RGBColor(0.3f, 0.92f, 0.55f), 4.0f);
    p1->shadows = false;
    add_light(p1);

    PointLight *p2 = new PointLight(Point3D(-30, 110, -80), RGBColor(0.85f, 0.55f, 0.95f), 3.5f);
    p2->shadows = false;
    add_light(p2);

    Directional *sun = new Directional(Vector3D(0.15, -1, -0.3), RGBColor(1, 0.95f, 0.9f), 1.6f);
    sun->shadows = false;
    add_light(sun);

    PointLight *fill = new PointLight(Point3D(0, 50, 90), RGBColor(0.85f, 0.82f, 0.78f), 2.5f);
    fill->shadows = false;
    add_light(fill);
}
