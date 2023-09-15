cmake_minimum_required(VERSION 3.15)
project(RobotDriver)

set(CMAKE_CXX_STANDARD 17)
# Support degug
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")

SET(CMAKE_BUILD_TYPE "Debug")
option(ENABLE_TEST "Build test" ON)

if(WIN32)
  # create symbol for exporting lib for dynmic library
  SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  # control where the static and shared libraries are built so that on windows
  # we don't need to tinker with the path to run the executable
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
  # Build static in windows
  option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
  # Set the runtime lib for MSCV (global)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
  option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
endif()

# Set the installation directory
SET(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/out/${PROJECT_NAME}-${CMAKE_BUILD_TYPE}")

if(ENABLE_TEST)
  include(CTest)
  find_package(GTest)
  include(GoogleTest)
endif()

add_subdirectory(src)
