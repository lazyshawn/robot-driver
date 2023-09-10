add_library(builder
  src/builder.cpp
)

# Usage requirement of lib
target_include_directories(builder PUBLIC
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
target_link_libraries(builder PUBLIC
  PahoMqttCpp::paho-mqttpp3
)


# Test
if(ENABLE_TEST)
  add_executable(test_builder
    test/test_builder.cpp
  )
  target_link_libraries(test_builder PUBLIC
    builder
    GTest::gtest GTest::gtest_main
  )
endif()
