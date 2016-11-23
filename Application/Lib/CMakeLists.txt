cmake_minimum_required (VERSION 3.5.0)
project (ApplicationLib)


# Application library source files
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
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC GraphicsLib)