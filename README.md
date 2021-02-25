# GLFW Starter

This is a simple demonstration of GLFW, ImGui, and OpenGL.


## Dependencies

1. relatively recent C/C++ compilers such as gcc/g++ (the app builds as C++17 by default, but this
    isn't technically required)
2. cmake 3.13 or newer
3. GLFW 3.3

## Building

This projects builds in the standard cmake fashion.

```
mkdir build
cd build
cmake ..
make
```

Protips:
- You can specify a particular build type using e.g. `cmake .. -DCMAKE_BUILD_TYPE=Debug`.
- You can build in parallel with `make -j`.
