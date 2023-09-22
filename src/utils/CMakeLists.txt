add_library(utils_socket
  src/socket.cpp
)

# Usage requirement of lib
target_include_directories(utils_socket PUBLIC
  # include directory in your build tree
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  # include directory when installed
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(ENABLE_TEST)
  add_executable(main
    src/main.cpp
  )
  target_link_libraries(main
    GTest::gtest GTest::gtest_main
  )
  gtest_discover_tests(main)
endif()

add_library(utils_interpolate
  src/interpolate.cpp
)

# Usage requirement of lib
target_include_directories(utils_interpolate PUBLIC
  # include directory in your build tree
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  # include directory when installed
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

###############################################################################
# Install package
###############################################################################
# Install public include files
install(DIRECTORY include/utils
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# We add `yourlibrary` target into the export set.
# The export set will contain all targets to be imported by the other project.
# It also installs the library to the install script so they are installed:
set(utils_libs utils_socket utils_interpolate)
install(TARGETS ${utils_libs} EXPORT RobotDriverTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
