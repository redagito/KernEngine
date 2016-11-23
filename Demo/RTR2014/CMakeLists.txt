cmake_minimum_required (VERSION 3.0.0)
project (DemoRTR2014)

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
)

add_executable (${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries (${PROJECT_NAME} GraphicsLib)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})