add_library(test_socket
  src/socket.cpp
)

# Usage requirement of lib
target_include_directories(test_socket PUBLIC
  # include directory in your build tree
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  # include directory when installed
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# include(GoogleTest)
find_package(GTest REQUIRED)
add_executable(main
  src/main.cpp
)
target_link_libraries(main
  GTest::gtest GTest::gtest_main
)
gtest_discover_tests(main)
