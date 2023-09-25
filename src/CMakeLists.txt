# Set the desired runtime path relative to the executable location
cmake_policy(SET CMP0095 NEW)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib:\${ORIGIN}/../lib:${CMAKE_SOURCE_DIR}/lib")

# Include gnuinstalldir to get the platform's standard directories:
include(GNUInstallDirs)

add_subdirectory(utils)
add_subdirectory(fanuc)
add_subdirectory(sbot)


if(EXPORT_CMAKE_PACKAGE)
  include(ExportPackage)
endif()
