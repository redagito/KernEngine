cmake_minimum_required(VERSION 3.25.0)
project(KernEngine)

# Demo project as default startup
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RenderTest)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CXX_STANDARD_REQUIRED ON)

# Testing
find_package(catch2 REQUIRED)

# Dependencies
# Foundation
find_package(glm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(fmtlog REQUIRED)

# Graphics
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(tinyobjloader REQUIRED)
find_package(assimp REQUIRED)
find_package(stb REQUIRED)
find_package(Freetype REQUIRED)
find_package(imgui REQUIRED)

# Audio
find_package(OpenAL REQUIRED)
find_package(vorbis REQUIRED)

# Scripting
find_package(lua REQUIRED)
find_package(sol2 REQUIRED)

# Foundation library
add_subdirectory(Foundation)

# Audio library
# Depends on foundation
add_subdirectory(Audio)

# Graphics and rendering library
# Depends on Foundation
add_subdirectory(Graphics)

# Application layer library
# Depends on Graphics
add_subdirectory(Application)

# Demo applications and sample games
# Depends on Application
add_subdirectory(Demo)