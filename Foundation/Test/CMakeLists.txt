# Boost libraries
set(Boost_USE_STATIC_LIBS ON CACHE BOOL "")
find_package(Boost 1.55 REQUIRED system unit_test_framework)


file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h 
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

set (TEST_NAME ${PROJECT_NAME}Test)
add_executable (${TEST_NAME} ${SOURCE_FILES})

# Require C++11
set_target_properties(${TEST_NAME} PROPERTIES
	CXX_STANDARD 11
)

# Library
target_link_libraries (${TEST_NAME}
	LINK_PRIVATE ${PROJECT_NAME}
	LINK_PRIVATE ${Boost_LIBRARIES}
)

target_include_directories(${TEST_NAME}
	PRIVATE ${Boost_INCLUDE_DIR}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source/
)