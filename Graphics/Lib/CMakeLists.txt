# OpenGL
find_package (OpenGL REQUIRED)

# tinyobjloader
add_subdirectory (external/tinyobjloader)

# GLFW
set (GLFW_BUILD_DOCS OFF CACHE BOOL "")
set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set (GLFW_BUILD_TESTS OFF CACHE BOOL "")
set (GLFW_INSTALL OFF CACHE BOOL "")
add_subdirectory (external/glfw)

# LodePNG as third party source
file (GLOB LODEPNG_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/external/lodepng/lodepng.cpp)
set (THIRD_PARTY_SOURCES ${THIRD_PARTY_SOURCES} ${LODEPNG_SOURCES})

# Flextgl 4.3 core
file (GLOB FLEXTGL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/external/flextgl43/*.cpp)

# Flextgl 4.2 core
# file (GLOB FLEXTGL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/external/flextgl42/*.cpp)

set (THIRD_PARTY_SOURCES ${THIRD_PARTY_SOURCES} ${FLEXTGL_SOURCES})


# Library source files
file (GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.hxx
)

add_library (${PROJECT_NAME} ${SOURCE_FILES} ${THIRD_PARTY_SOURCES})


# Properties
set_target_properties (${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 11
)


# Includes
target_include_directories(${PROJECT_NAME} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include

	PUBLIC ${OPENGL_INCLUDE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/lodepng
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/flextgl43
	# PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/flextgl42
)


# Link dependencies
target_link_libraries(${PROJECT_NAME}
	LINK_PUBLIC Foundation

	LINK_PUBLIC ${OPENGL_gl_LIBRARY}
	LINK_PUBLIC tinyobjloader
	LINK_PUBLIC ${GLFW_LIBRARIES}
	LINK_PUBLIC glfw
)