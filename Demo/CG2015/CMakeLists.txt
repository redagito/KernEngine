cmake_minimum_required (VERSION 3.0.0)
project (DemoCG2015)

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
)

add_executable (${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries (${PROJECT_NAME} ApplicationLib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})