# KernEngine

![Screenshot cg15 2](./doc/screenshot_cg2015_2.jpg)
> The GL in OpenGL stands for "Good Luck" because you are going to need it.
![Screenshot rtr14 2](./doc/screenshot_rtr2014_2.jpg)
KernEngine is a Game Engine/Framework for realtime 3d rendering and game development.
Extracted from 2 projects for university courses.

## Requirements

Development is done with
* Conan 2.x
* CMake 3.27.x
* Visual Studio 2022 Community Edition

## Building

Conan install for debug and release build types
```
conan install . --build=missing --settings=build_type=Debug && conan install . --build=missing --settings=build_type=Release
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

## Literature

Relevant tutorials
* https://learnopengl.com
* http://www.opengl-tutorial.org
* https://open.gl
* https://ogldev.org/
* https://www.spieleprogrammierung.net/p/opengl-tutorial-liste.html
* https://www.tomdalling.com/blog/category/modern-opengl
* http://lazyfoo.net/tutorials/OpenGL
* https://antongerdelan.net/opengl
* http://www.mbsoftworks.sk/tutorials
* http://duriansoftware.com/joe/An-intro-to-modern-OpenGL.-Table-of-Contents.html
* https://www.rastertek.com/tutdx11s2ter.html - Actually DirectX11 but the terrain tutorial looks interesting
* https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions
* https://www.swiftless.com/
* https://www.khronos.org/opengl/wiki/Getting_Started - List of other tutorials
* https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions

## Screenshots

![Screenshot rtr14 1](./doc/screenshot_rtr2014_1.jpg)
![Screenshot rtr14 3](./doc/screenshot_rtr2014_3.jpg)
![Screenshot cg15 1](./doc/screenshot_cg2015_1.jpg)
![Screenshot cg15 3](./doc/screenshot_cg2015_3.jpg)