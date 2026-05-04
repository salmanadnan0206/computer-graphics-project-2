#include "../cameras/Perspective.hpp"
#include "../geometry/Triangle.hpp"
#include "../geometry/Sphere.hpp"
#include "../geometry/Plane.hpp"
#include "../materials/Cosine.hpp"
#include "../materials/Matte.hpp"
#include "../materials/Phong.hpp"
#include "../materials/Reflective.hpp"
#include "../samplers/Simple.hpp"
#include "../tracers/Whitted.hpp"
#include "../lights/PointLight.hpp"
#include "../lights/Directional.hpp"
#include "../utilities/Constants.hpp"
#include "../world/World.hpp"

#include <iostream>

void World::build(void) {
    vplane.top_left.x = -12;
    vplane.top_left.y = 9;
    vplane.top_left.z = 10;
    vplane.bottom_right.x = 12;
    vplane.bottom_right.y = -9;
    vplane.bottom_right.z = 10;
    vplane.hres = 200;
    vplane.vres = 150;

    bg_color = RGBColor(0.1f, 0.1f, 0.2f);
    ambient = RGBColor(0.3f, 0.3f, 0.3f);

    set_camera(new Perspective(0, 55, 95));
    sampler_ptr = new Simple(camera_ptr, &vplane);
    tracer_ptr = new Whitted(this, 2);

    // bright red floor using triangles (same as cavern)
    Material *floor_mat = new Matte(0.3f, 0.7f, red);

    // floor quad (normal up)
    float xmin=-200, xmax=200, zmin=-350, zmax=120;
    Triangle *f1 = new Triangle(Point3D(xmin, 0, zmax), Point3D(xmax, 0, zmax), Point3D(xmax, 0, zmin));
    Triangle *f2 = new Triangle(Point3D(xmin, 0, zmax), Point3D(xmax, 0, zmin), Point3D(xmin, 0, zmin));
    f1->set_material(floor_mat); f2->set_material(floor_mat);
    add_geometry(f1); add_geometry(f2);

    // one crystal
    Sphere *s = new Sphere(Point3D(0, 15, 0), 12);
    s->set_material(new Phong(0.2f, 0.5f, 0.5f, blue, 80));
    add_geometry(s);

    Directional *d = new Directional(Vector3D(0, -1, 0), white, 3.0f);
    d->shadows = false;
    add_light(d);

    add_light(new PointLight(Point3D(0, 50, 50), white, 10.0f));
}
