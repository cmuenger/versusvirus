# CMake version check
cmake_minimum_required(VERSION 3.13)

# Create project 
project(versusvirus)           

# Enable c++14 standard
set(CMAKE_CXX_STANDARD 17)      

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Find libpng
find_package(PNG REQUIRED)

# Link libpng to executable
macro (add_executable_png _name)
    # invoke built-in add_executable
    add_executable(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_name} ${PNG_LIBRARIES})		
    endif()
endmacro()

# Include src directory
add_subdirectory(src)