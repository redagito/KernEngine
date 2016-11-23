cmake_minimum_required (VERSION 3.5.0)
project (GraphicsLib)


# OpenGL
find_package(OpenGL REQUIRED)

# tinyobjloader
add_subdirectory(external/tinyobjloader)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")
add_subdirectory(external/glfw)


# LodePNG as third party source
file(GLOB LODEPNG_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/external/lodepng/lodepng.cpp)
set(THIRD_PARTY_SOURCES ${THIRD_PARTY_SOURCES} ${LODEPNG_SOURCES})

# Flextgl 4.3 core
file(GLOB FLEXTGL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/external/flextgl43/*.cpp)
set(THIRD_PARTY_SOURCES ${THIRD_PARTY_SOURCES} ${FLEXTGL_SOURCES})

# Foundation library source files
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

add_library (${PROJECT_NAME} ${SOURCE_FILES} ${THIRD_PARTY_SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

# Private includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Public includes
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


# Foundation library
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC FoundationLib)

# OpenGL
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC  ${OPENGL_gl_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})

# LodePNG
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/lodepng)

# TinyOBJLoader
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC tinyobjloader)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader)

# GLFW
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC glfw)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include)

# FlextGL
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/flextgl43)