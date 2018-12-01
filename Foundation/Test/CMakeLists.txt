set (TEST_NAME ${PROJECT_NAME}Test)

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h 
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cxx
)
GroupSources(source)

add_executable (${TEST_NAME} ${SOURCE_FILES})
add_test (${TEST_NAME} ${TEST_NAME})

set_target_properties(${TEST_NAME} PROPERTIES
	CXX_STANDARD 17
)

target_link_libraries (${TEST_NAME}
	LINK_PRIVATE ${PROJECT_NAME}
	LINK_PRIVATE CONAN_PKG::catch2
)

target_include_directories(${TEST_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source/
)