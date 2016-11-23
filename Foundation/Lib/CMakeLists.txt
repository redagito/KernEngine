cmake_minimum_required (VERSION 3.5.0)
project (FoundationLib)


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

# Crypto++
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC cryptopp-static)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/cryptopp)

# Boost
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})

# GLM
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glm)
target_compile_definitions(${PROJECT_NAME} PUBLIC -DGLM_ENABLE_EXPERIMENTAL)

# JsonCPP
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC jsoncpp_lib_static)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/jsoncpp/include)

# TinyXML2
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC tinyxml2_static)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml2)


# SQLite3
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/sqlite-amalgamation-3150100)

# Additional libraries for SQLite3 (g++/clang only)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_link_libraries(${PROJECT_NAME} LINK_PUBLIC dl pthread)
endif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")