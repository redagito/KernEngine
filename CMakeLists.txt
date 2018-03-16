cmake_minimum_required (VERSION 3.0.0)
project (KernEngine)

# Use folder structure
set_property (GLOBAL PROPERTY USE_FOLDERS ON)

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