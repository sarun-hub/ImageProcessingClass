### How to use
1. Create build directory
```
mkdir build
```

2. Set CMake config (at workspace root) 

```
cmake -S . -B build
```

3. Build from workspace root
```
cmake --build build
```

1. Run executable in `build` that you want, eg. run lec2_hello_world/rgb_to_grey

```
./build/lec2_hello_world/rgb_to_grey ImageSample/Airplane.ppm
```