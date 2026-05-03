# CS440 Project 2 - Ray Tracer

Team 07885 / 07879 / 09212

## Build

From this folder:

```bash
g++ -O3 -fopenmp -std=c++17 \
    raytracer.cpp \
    world/*.cpp utilities/*.cpp geometry/*.cpp cameras/*.cpp \
    image/*.cpp samplers/*.cpp materials/*.cpp brdf/*.cpp \
    lights/*.cpp tracers/*.cpp acceleration/*.cpp \
    build/buildCrystalCavern.cpp \
    -o scene.exe && ./scene.exe
```

Swap the build file for whichever scene you want. Output goes to scene.png.

## Build files

- buildCrystalCavern.cpp - 1920x1080 final, BVH + jitter 2x2
- buildCrystalCavernMid.cpp - 1280x720, BVH, simple sampler
- buildCrystalCavernLow.cpp - 480x360, BVH, fast preview
- buildCrystalCavernNoAccel.cpp - 20x15, no BVH, for timing
- buildHelloWorld.cpp - hw5 regression
- buildMvp.cpp - hw5 regression
- buildChapter14.cpp - hw5 regression

## hw5 reference outputs

scene-1.png, scene-2.png, scene-3.png are the locked reference outputs from hw5. They should match exactly after compilation.

## Notes

- BVH is opt in. Call build_accel() in your build file, or set use_accel=false to skip it.
- Tracer is set via world.tracer_ptr. Null falls back to original hw5 shading.
- Sampler is world.sampler_ptr. Simple or Jittered(n) for NxN AA.
- OpenMP: compile with -fopenmp, loop is already parallelized.
