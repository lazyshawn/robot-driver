add_executable(fanuc_driver
  src/driver_core.cpp
)

# Usage requirement of lib
target_include_directories(fanuc_driver PUBLIC
  # include directory in your build tree
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  # include directory when installed
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(fanuc_driver PUBLIC
  utils_socket
)

# find EIPScanner
set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${CMAKE_SOURCE_DIR}/lib/EIPScanner/lib/pkgconfig")
find_package(PkgConfig REQUIRED)
pkg_search_module(EIP_LIB REQUIRED IMPORTED_TARGET EIPScanner)

add_executable(fanuc_eip_driver
  src/eip_driver.cpp
)
target_include_directories(fanuc_eip_driver PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(fanuc_eip_driver PUBLIC
  PkgConfig::EIP_LIB
)
