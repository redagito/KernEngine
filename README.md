# KernEngine

Game Engine/Framework

Extracted from 2 projects for university courses.

## Building

Conan install for debug and release build types
```
conan install . --output-folder=build --build=missing --settings=build_type=Debug && conan install . --output-folder=build --build=missing --settings=build_type=Release
```

CMake project generation
```
cmake --preset conan-default
```

Build in release mode
```
cd build
cmake --build . --config Release
```