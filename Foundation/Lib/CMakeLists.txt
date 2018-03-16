# JsonCPP
set(JSONCPP_WITH_TESTS OFF CACHE BOOL "")
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "")
set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE BOOL "")
set(JSONCPP_WITH_WARNING_AS_ERROR OFF CACHE BOOL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/jsoncpp)

# Crypto++ library
set(BUILD_SHARED OFF CACHE BOOL "")
set(BUILD_TESTING OFF CACHE BOOL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/cryptopp)

# TinyXML2
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(BUILD_STATIC_LIBS ON CACHE BOOL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml2)

# Boost libraries
set(Boost_USE_STATIC_LIBS ON CACHE BOOL "")
find_package(Boost 1.55 REQUIRED system program_options regex filesystem date_time log)


# Third party code
# SQLite3
file(GLOB SQLITE3_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/external/sqlite-amalgamation-3081101/sqlite3.c)
set(THIRD_PARTY_SOURCES ${THIRD_PARTY_SOURCES} ${SQLITE3_SOURCES})


# Foundation library source files
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

# Foundation library
add_library (${PROJECT_NAME} ${SOURCE_FILES} ${THIRD_PARTY_SOURCES})


# Properties
set_target_properties (${PROJECT_NAME} PROPERTIES 
	CXX_STANDARD 11
)


# Link dependencies
target_link_libraries(${PROJECT_NAME} 
	LINK_PUBLIC cryptopp-static
	LINK_PUBLIC ${Boost_LIBRARIES}
	LINK_PUBLIC jsoncpp_lib_static
	LINK_PUBLIC tinyxml2_static	
)


# Additional libraries for SQLite3 (g++/clang only)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_link_libraries(${PROJECT_NAME} LINK_PUBLIC dl pthread)
endif ()


# Includes
target_include_directories(${PROJECT_NAME} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/cryptopp
	PUBLIC ${Boost_INCLUDE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/jsoncpp/include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml2
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/sqlite-amalgamation-3150100
)


# Additional definitions
target_compile_definitions(${PROJECT_NAME} 
	PUBLIC -DGLM_ENABLE_EXPERIMENTAL
)