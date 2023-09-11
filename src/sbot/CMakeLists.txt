add_library(sbot_builder
  src/builder.cpp
)

# Usage requirement of lib
target_include_directories(sbot_builder PUBLIC
  # include directory in your build tree
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  # include directory when installed
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

#find PahoMqttCpp
list(APPEND CMAKE_PREFIX_PATH "/home/shawn/Desktop/mqtt/eclipse-paho-mqtt-c")
# list(APPEND CMAKE_PREFIX_PATH "../lib")
# find_package(eclipse-paho-mqtt-c PATHS ${CMAKE_SOURCE_DIR}/lib)
find_package(nolhmann-json PATHS ${CMAKE_SOURCE_DIR}/lib)
find_package(PahoMqttCpp PATHS ${CMAKE_SOURCE_DIR}/lib REQUIRED)
target_link_libraries(sbot_builder PUBLIC
  PahoMqttCpp::paho-mqttpp3
  httplib
)


# Test
if(ENABLE_TEST)
  add_executable(test_builder
    test/test_builder.cpp
  )
  target_link_libraries(test_builder PUBLIC
    sbot_builder
    GTest::gtest GTest::gtest_main
  )
  gtest_discover_tests(test_builder)
endif()

###############################################################################
# Install package
###############################################################################
# Install public include files
install(DIRECTORY include/sbot
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# We add `yourlibrary` target into the export set.
# The export set will contain all targets to be imported by the other project.
# It also installs the library to the install script so they are installed:
set(sbot_libs sbot_builder)
install(TARGETS ${sbot_libs} EXPORT RobotDriverTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
