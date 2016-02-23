cmake_minimum_required(VERSION 3.0)

project(lattice-fitting)

# add custom find module for NTL
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (NOT MSVC)
  list(APPEND CMAKE_CXX_FLAGS "-std=c++14 -Wall")
endif()

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(NTL REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory(src)

option(BUILD_TESTS "Build the tests" ON)

if (BUILD_TESTS)
  include_directories(src)
  add_subdirectory(tests)
endif()
