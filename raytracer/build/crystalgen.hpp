#pragma once

#include "../geometry/Triangle.hpp"
#include "../utilities/Constants.hpp"
#include "../world/World.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

// simple deterministic rng so renders are repeatable
static unsigned int gseed = 1337;
static float rnd() {
    gseed = gseed * 1103515245u + 12345u;
    return (float)((gseed >> 16) & 0x7FFF) / 32767.0f;
}
static float rnd(float lo, float hi) { return lo + rnd() * (hi - lo); }

static void add_crystal(World &w, Point3D base, float h, float r, Material *mat, bool down) {
    float bh = h * 0.6f;
    float tr = r * 0.55f;
    float dir = down ? -1.0f : 1.0f;

    Point3D bot[6], top[6];
    for (int i = 0; i < 6; i++) {
        float a = i * PI / 3.0f;
        bot[i] = Point3D(base.x + r * cosf(a),  base.y,           base.z + r * sinf(a));
        top[i] = Point3D(base.x + tr * cosf(a), base.y + dir * bh, base.z + tr * sinf(a));
    }
    Point3D tip(base.x, base.y + dir * h, base.z);

    for (int i = 0; i < 6; i++) {
        int j = (i + 1) % 6;
        Triangle *s, *t, *u;
        if (!down) {
            s = new Triangle(bot[i], top[i], top[j]);
            t = new Triangle(bot[i], top[j], bot[j]);
            u = new Triangle(top[i], tip,    top[j]);
        } else {
            s = new Triangle(bot[i], top[j], top[i]);
            t = new Triangle(bot[i], bot[j], top[j]);
            u = new Triangle(top[i], top[j], tip);
        }
        s->set_material(mat); t->set_material(mat); u->set_material(mat);
        w.add_geometry(s); w.add_geometry(t); w.add_geometry(u);
    }
}

static void add_cave_walls(World &w, Material *fm, Material *wm,
                           float x0, float x1, float z0, float z1, float fy, float cy) {
    auto quad = [&](Point3D a, Point3D b, Point3D c, Point3D d, Material *m) {
        Triangle *p = new Triangle(a, b, c);
        Triangle *q = new Triangle(a, c, d);
        p->set_material(m); q->set_material(m);
        w.add_geometry(p); w.add_geometry(q);
    };

    quad(Point3D(x0,fy,z1), Point3D(x1,fy,z1), Point3D(x1,fy,z0), Point3D(x0,fy,z0), fm); // floor
    quad(Point3D(x0,cy,z0), Point3D(x1,cy,z0), Point3D(x1,cy,z1), Point3D(x0,cy,z1), wm); // ceiling
    quad(Point3D(x1,fy,z0), Point3D(x1,cy,z0), Point3D(x0,cy,z0), Point3D(x0,fy,z0), wm); // back
    quad(Point3D(x0,fy,z0), Point3D(x0,cy,z0), Point3D(x0,cy,z1), Point3D(x0,fy,z1), wm); // left
    quad(Point3D(x1,fy,z1), Point3D(x1,cy,z1), Point3D(x1,cy,z0), Point3D(x1,fy,z0), wm); // right
}

static bool in_corridor(float x, float z, float z1) {
    float f = (z1 - z) / (z1 + 350.0f);
    if (f < 0) f = 0;
    if (f > 1) f = 1;
    float hw = 20.0f + f * 60.0f;
    return std::fabs(x) < hw;
}

static void populate_crystals(World &w, float x0, float x1, float z0, float z1, float fy, float cy) {
    Material *mc[6];
    mc[0] = new Phong(0.18f, 0.45f, 0.6f,  RGBColor(0.92f, 0.94f, 0.97f), 110); // white
    mc[1] = new Phong(0.18f, 0.4f,  0.55f, RGBColor(0.45f, 0.65f, 0.95f), 80);  // blue
    mc[2] = new Phong(0.18f, 0.4f,  0.5f,  RGBColor(0.7f,  0.4f,  0.92f), 70);  // purple
    mc[3] = new Phong(0.22f, 0.5f,  0.45f, RGBColor(0.95f, 0.75f, 0.25f), 60);  // amber
    mc[4] = new Phong(0.18f, 0.4f,  0.5f,  RGBColor(0.35f, 0.88f, 0.7f),  90);  // teal
    mc[5] = new Phong(0.2f,  0.45f, 0.5f,  RGBColor(0.95f, 0.55f, 0.7f),  80);  // pink

    // bias toward white and blue
    Material *pool[] = { mc[0],mc[0], mc[1],mc[1],mc[1], mc[2],mc[2], mc[3], mc[4],mc[4], mc[5] };
    int np = 11;

    std::cout << "placing floor clusters...\n";
    for (int cl = 0; cl < 1100; cl++) {
        float cx = rnd(x0+8, x1-8);
        float cz = rnd(z0+8, z1-8);
        if (in_corridor(cx, cz, z1)) continue;
        int cnt = (int)rnd(20, 60);
        Material *m = pool[rand() % np];
        float bh = rnd(8, 45), br = rnd(1.2f, 5.5f);
        for (int k = 0; k < cnt; k++) {
            float px = cx + rnd(-15, 15);
            float pz = cz + rnd(-15, 15);
            if (px < x0+3 || px > x1-3 || pz < z0+3 || pz > z1-3) continue;
            if (in_corridor(px, pz, z1)) continue;
            add_crystal(w, Point3D(px, fy, pz), bh*rnd(0.3f,1.6f), br*rnd(0.4f,1.4f), m, false);
        }
    }

    std::cout << "placing ceiling clusters...\n";
    for (int cl = 0; cl < 700; cl++) {
        float cx = rnd(x0+8, x1-8);
        float cz = rnd(z0+8, z1-8);
        int cnt = (int)rnd(15, 45);
        Material *m = pool[rand() % np];
        float bh = rnd(6, 35), br = rnd(1.0f, 4.5f);
        for (int k = 0; k < cnt; k++) {
            float px = cx + rnd(-12, 12);
            float pz = cz + rnd(-12, 12);
            if (px < x0+3 || px > x1-3 || pz < z0+3 || pz > z1-3) continue;
            add_crystal(w, Point3D(px, cy, pz), bh*rnd(0.3f,1.5f), br*rnd(0.4f,1.3f), m, true);
        }
    }

    for (int i = 0; i < 16000; i++) {
        float px = rnd(x0+3, x1-3);
        float pz = rnd(z0+3, z1-3);
        if (in_corridor(px, pz, z1)) continue;
        add_crystal(w, Point3D(px, fy, pz), rnd(2,22), rnd(0.5f,3.5f), pool[rand()%np], false);
    }
    for (int i = 0; i < 9000; i++) {
        float px = rnd(x0+3, x1-3);
        float pz = rnd(z0+3, z1-3);
        add_crystal(w, Point3D(px, cy, pz), rnd(2,18), rnd(0.5f,3.0f), pool[rand()%np], true);
    }
}
