cmake_minimum_required (VERSION 3.12.0)
project (KernEngine)

# Virtual folders in visual studio
include (${PROJECT_SOURCE_DIR}/cmake/GroupSources.cmake)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.12/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

# Use folder structure
set_property (GLOBAL PROPERTY USE_FOLDERS ON)
set (CXX_STANDARD_REQUIRED ON)

# Provides unit test library in global scope
conan_cmake_run(REQUIRES 
	catch2/2.2.2@bincrafters/stable
	
	BASIC_SETUP CMAKE_TARGETS
	BUILD missing
)
set_target_properties (CONAN_PKG::catch2 PROPERTIES IMPORTED_GLOBAL TRUE)

# Foundation library
add_subdirectory (Foundation)

# Graphics and rendering library
add_subdirectory (Graphics)

# Application layer library
add_subdirectory (Application)

# Utility library for toolkit
add_subdirectory (Util)

# Tools and applications
add_subdirectory (Toolkit)

# Demo
add_subdirectory (Demo)