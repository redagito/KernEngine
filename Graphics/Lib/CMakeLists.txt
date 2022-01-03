find_package (OpenGL REQUIRED)

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
GroupSources(source)
GroupSources(include)

add_library (${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties (${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
)

conan_cmake_run(REQUIRES
	glfw/3.3.6
	glad/0.1.34
	lodepng/cci.20200615
	assimp/5.1.0
	
	OPTIONS glad:spec=gl
	OPTIONS glad:gl_profile=core
	OPTIONS glad:gl_version=4.6
	OPTIONS glad:no_loader=False
	
	OPTIONS assimp:shared=False
	
	BASIC_SETUP CMAKE_TARGETS
	BUILD missing
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC Foundation

	PUBLIC ${OPENGL_gl_LIBRARY}
	PUBLIC CONAN_PKG::glfw
	PUBLIC CONAN_PKG::glad
	PUBLIC CONAN_PKG::lodepng
	PUBLIC CONAN_PKG::assimp
)

target_include_directories(${PROJECT_NAME} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include

	PUBLIC ${OPENGL_INCLUDE_DIR}
)