cmake_minimum_required(VERSION 3.25.0)
project(KernEngine)

# Use folder structure
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CXX_STANDARD_REQUIRED ON)

# Dependencies
# Foundation
find_package(glm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(SQLite3 REQUIRED)

# Graphics
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(assimp REQUIRED)
find_package(stb REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenAL REQUIRED)
find_package(vorbis REQUIRED)
find_package(lua REQUIRED)
find_package(sol2 REQUIRED)
find_package(fmtlog REQUIRED)
find_package(catch2 REQUIRED)

# Foundation library
add_subdirectory (Foundation)

# Graphics and rendering library
# Depends on Foundation
add_subdirectory (Graphics)

# Application layer library
# Depends on Graphics
add_subdirectory (Application)

# Demo applications and sample games
# Depends on Applicaton
# add_subdirectory (Demo)