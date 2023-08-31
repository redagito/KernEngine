cmake_minimum_required(VERSION 3.25.0)
project(KernEngine)

# Demo project as default startup
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RenderDemo)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CXX_STANDARD_REQUIRED ON)

if (MSVC)
	# Ignore warnings about missing pdb
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4099")
	set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4099")
endif()


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
find_package(minimp3 REQUIRED)

# Scripting
find_package(lua REQUIRED)
find_package(sol2 REQUIRED)

# Engine
add_subdirectory(Engine)

# Demo applications and sample games
# Depends on Engine
add_subdirectory(Demo)