cmake_minimum_required(VERSION 3.15)
project(robot-driver)

set(CMAKE_CXX_STANDARD 17)
# Support degug
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")

option(ENABLE_TEST "Build test" ON)

if(ENABLE_TEST)
  include(CTest)
  find_package(GTest)
  include(GoogleTest)
endif()

add_subdirectory(src)
