# CS440 Project 2 – Ray Tracing Engine

Team: 07885 / 07879 / 09212

---

## How to Build

Open a terminal, go into this folder (the one with raytracer.cpp), and run:

```bash
g++ -O3 -fopenmp -std=c++17 \
    raytracer.cpp \
    world/*.cpp utilities/*.cpp geometry/*.cpp cameras/*.cpp \
    image/*.cpp samplers/*.cpp materials/*.cpp brdf/*.cpp \
    lights/*.cpp tracers/*.cpp acceleration/*.cpp \
    build/buildCrystalCavern.cpp \
    -o scene.exe && ./scene.exe
```

The output image is written to `scene.png` in whatever directory you ran the command from.

Just swap out `build/buildCrystalCavern.cpp` for whichever scene you want to render (see the list below).

If your compiler doesn't support `-fopenmp` you can drop that flag — it'll still work, just slower since it won't use multiple threads.

---

## Build Files

These are all in the `build/` folder:

| File | Resolution | BVH | Sampler | Notes |
|---|---|---|---|---|
| buildCrystalCavern.cpp | 1920×1080 | on | Jittered 2×2 | final high quality render |
| buildCrystalCavernMid.cpp | 1280×720 | on | Simple | decent quality, ~1 min |
| buildCrystalCavernLow.cpp | 480×360 | on | Simple | fast preview |
| buildCrystalCavernNoAccel.cpp | 20×15 | **off** | Simple | brute force, for timing only |
| buildHelloWorld.cpp | — | — | — | hw5 regression check |
| buildMvp.cpp | — | — | — | hw5 regression check |
| buildChapter14.cpp | — | — | — | hw5 regression check |

The three hw5 regression builds should produce outputs that match `scene-1.png`, `scene-2.png`, and `scene-3.png` exactly.

---

## Turning the BVH On or Off

By default, the BVH is **off**. To turn it on, call `world.build_accel()` inside your build file after adding all your geometry. That's it. Most of our Crystal Cavern build files already do this.

If you want to render without the BVH (brute force intersection against every primitive), just don't call `build_accel()`, or set `world.use_accel = false` manually. We use this in `buildCrystalCavernNoAccel.cpp` to get the timing numbers for the acceleration comparison table on the website.

---

## Changing the Sampler

The sampler is set in the build file via `world.sampler_ptr`. Two options:

- `new Simple(...)` — one ray per pixel, fastest
- `new Jittered(camera, vplane, n)` — fires n×n rays per pixel with a small random jitter in each grid cell, reduces aliasing. We used n=2 (4 samples/pixel) for the final render.

---

## Timing Reference

Measured on an 8-thread machine with -O3 and OpenMP:

| Scene | BVH | Resolution | Time |
|---|---|---|---|
| Crystal Cavern | off | 20×15 | ~50 s (166 ms/pixel) |
| Crystal Cavern | on | 480×360 | ~7 s (0.04 ms/pixel) |
| Crystal Cavern | on | 1280×720 | ~57 s |
| Crystal Cavern | on | 1920×1080, 4spp | ~9 min |

Per-pixel speedup from the BVH: roughly **4000×** on this scene (1.3 million triangles).

---

## Dependencies

Just a C++17 compiler. No external libraries. stb_image_write is bundled in the `image/` folder for PNG output.
