cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED CMAKE_CXX_CLANG_TIDY)
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
endif()

find_package(Boost REQUIRED COMPONENTS program_options serialization)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(geometry)
add_subdirectory(graph)
add_subdirectory(multi_array)
add_subdirectory(program_options)
add_subdirectory(property_tree)
add_subdirectory(serialization)
add_subdirectory(units)
