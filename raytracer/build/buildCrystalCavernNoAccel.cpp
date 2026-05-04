#include "../cameras/Perspective.hpp"
#include "../geometry/Triangle.hpp"
#include "../materials/Matte.hpp"
#include "../materials/Phong.hpp"
#include "../materials/Reflective.hpp"
#include "../samplers/Simple.hpp"
#include "../tracers/Whitted.hpp"
#include "../lights/PointLight.hpp"
#include "../lights/Directional.hpp"
#include "../utilities/Constants.hpp"
#include "../world/World.hpp"

#include "crystalgen.hpp"

#include <iostream>

void World::build(void) {
    srand(42);

    float xmin = -200, xmax = 200;
    float zmin = -350, zmax = 120;
    float floor_y = 0, ceil_y = 140;

    vplane.top_left.x = -26;
    vplane.top_left.y = 55;
    vplane.top_left.z = 10;
    vplane.bottom_right.x = 26;
    vplane.bottom_right.y = 15;
    vplane.bottom_right.z = 10;
    vplane.hres = 20;
    vplane.vres = 15;

    bg_color = RGBColor(0.01f, 0.01f, 0.02f);
    ambient = RGBColor(0.25f, 0.25f, 0.3f);

    set_camera(new Perspective(0, 70, 110));
    sampler_ptr = new Simple(camera_ptr, &vplane);
    tracer_ptr = new Whitted(this, 2);

    use_accel = false;

    Material *floor_mat = new Reflective(0.15f, 0.35f, 0.2f,
                                         RGBColor(0.18f, 0.18f, 0.22f), 50, 0.3f);
    Material *wall_mat = new Matte(0.3f, 0.55f, RGBColor(0.28f, 0.24f, 0.2f));

    add_cave_walls(*this, floor_mat, wall_mat, xmin, xmax, zmin, zmax, floor_y, ceil_y);
    populate_crystals(*this, xmin, xmax, zmin, zmax, floor_y, ceil_y);

    std::cout << "Geometry count: " << geometry.size() << "\n";
    std::cout << "Acceleration: OFF (brute force)\n";

    add_light(new PointLight(Point3D(-50, 60, -30), RGBColor(1.0f, 0.8f, 0.45f), 5.0f));
    add_light(new PointLight(Point3D(60, 70, -120), RGBColor(0.45f, 0.65f, 1.0f), 4.5f));

    PointLight *p1 = new PointLight(Point3D(20, 35, -220), RGBColor(0.3f, 0.92f, 0.55f), 4.0f);
    p1->shadows = false; add_light(p1);

    Directional *d = new Directional(Vector3D(0.15, -1, -0.3), RGBColor(1, 0.95f, 0.9f), 1.6f);
    d->shadows = false; add_light(d);
}
