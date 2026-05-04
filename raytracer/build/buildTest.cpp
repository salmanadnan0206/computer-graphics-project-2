#include "../cameras/Perspective.hpp"
#include "../geometry/Plane.hpp"
#include "../geometry/Sphere.hpp"
#include "../geometry/Triangle.hpp"
#include "../materials/Cosine.hpp"
#include "../materials/Matte.hpp"
#include "../materials/Phong.hpp"
#include "../materials/Reflective.hpp"
#include "../samplers/Simple.hpp"
#include "../samplers/Jittered.hpp"
#include "../tracers/Whitted.hpp"
#include "../lights/PointLight.hpp"
#include "../lights/Directional.hpp"
#include "../utilities/Constants.hpp"
#include "../world/World.hpp"

void World::build(void) {
    vplane.top_left.x = -10;
    vplane.top_left.y = 10;
    vplane.top_left.z = 10;
    vplane.bottom_right.x = 10;
    vplane.bottom_right.y = -10;
    vplane.bottom_right.z = 10;
    vplane.hres = 400;
    vplane.vres = 400;

    bg_color = RGBColor(0.05f, 0.05f, 0.08f);
    ambient = RGBColor(0.15f, 0.15f, 0.15f);

    set_camera(new Perspective(0, 3, 20));
    sampler_ptr = new Jittered(camera_ptr, &vplane, 2);
    tracer_ptr = new Whitted(this, 3);

    // red matte sphere
    Sphere *s1 = new Sphere(Point3D(-4, 2, 0), 3);
    s1->set_material(new Matte(0.2f, 0.8f, red));
    add_geometry(s1);

    // shiny blue phong sphere
    Sphere *s2 = new Sphere(Point3D(4, 1.5, -2), 2.5);
    s2->set_material(new Phong(0.15f, 0.6f, 0.4f, blue, 50));
    add_geometry(s2);

    // mirror sphere
    Sphere *s3 = new Sphere(Point3D(0, 1, 5), 1.5);
    s3->set_material(new Reflective(0.1f, 0.2f, 0.3f, RGBColor(0.8f), 80, 0.6f));
    add_geometry(s3);

    // ground plane
    Plane *floor = new Plane(Point3D(0, -1, 0), Vector3D(0, 1, 0));
    floor->set_material(new Matte(0.2f, 0.6f, RGBColor(0.4f, 0.4f, 0.35f)));
    add_geometry(floor);

    // lights
    add_light(new PointLight(Point3D(-10, 15, 10), RGBColor(1, 0.95f, 0.8f), 1.5f));
    add_light(new Directional(Vector3D(1, -1, -0.5), RGBColor(0.6f, 0.7f, 1.0f), 0.4f));
}
