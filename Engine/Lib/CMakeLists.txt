project(EngineLib)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)
# Source group to preserve folder structure in ide
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

add_library(${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES 
	CXX_STANDARD 17
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC glm::glm
	PUBLIC nlohmann_json::nlohmann_json
	PUBLIC fmtlog::fmtlog
	PUBLIC SQLite::SQLite3
    
	PUBLIC OpenAL::OpenAL
    # TODO Should be private?
	PUBLIC vorbis::vorbis
	PUBLIC minimp3::minimp3
    
	PUBLIC OpenGL::GL
	PUBLIC glfw
	PUBLIC glad::glad
	PUBLIC imgui::imgui
	PUBLIC stb::stb
	PUBLIC assimp::assimp
	PUBLIC tinyobjloader::tinyobjloader
	PUBLIC Freetype::Freetype
    
	PUBLIC lua::lua
	PUBLIC sol2::sol2
)

target_include_directories(${PROJECT_NAME} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)