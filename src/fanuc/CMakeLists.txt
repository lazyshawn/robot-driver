add_executable(fanuc-driver
  src/driver_core.cpp
)

# Usage requirement of lib
target_include_directories(fanuc-driver PUBLIC
  # include directory in your build tree
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  # include directory when installed
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(fanuc-driver PUBLIC
  utils_socket
)
