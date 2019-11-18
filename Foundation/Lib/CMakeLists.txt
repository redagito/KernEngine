file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.hxx
)
GroupSources(source)
GroupSources(include)

add_library (${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties (${PROJECT_NAME} PROPERTIES 
	CXX_STANDARD 17
)

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(REQUIRES 
	glm/0.9.9.5@g-truc/stable
	jsonformoderncpp/3.7.0@vthiery/stable
	pugixml/1.10@bincrafters/stable
	sqlite3/3.29.0@bincrafters/stable
	
	BASIC_SETUP CMAKE_TARGETS
	BUILD missing
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC CONAN_PKG::glm
	PUBLIC CONAN_PKG::jsonformoderncpp
	PUBLIC CONAN_PKG::sqlite3
	PUBLIC CONAN_PKG::pugixml
)

target_include_directories(${PROJECT_NAME} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(${PROJECT_NAME} 
	PUBLIC -DGLM_ENABLE_EXPERIMENTAL
)