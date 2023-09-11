# Set the desired runtime path relative to the executable location
cmake_policy(SET CMP0095 NEW)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib:\${ORIGIN}/../lib:${CMAKE_SOURCE_DIR}/lib")

# Include gnuinstalldir to get the platform's standard directories:
include(GNUInstallDirs)

add_subdirectory(utils)
# add_subdirectory(fanuc)
add_subdirectory(sbot)

###############################################################################
# Install package
###############################################################################
# Include gnuinstalldir to get the platform's standard directories:
# include(GNUInstallDirs)

# Now, we install the export set. This will generate a CMake file exporting all the target for other projects to use:
install(EXPORT RobotDriverTargets
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/RobotDriver"
  NAMESPACE RobotDriver::
)

# Now, we also export the current buildtree. Other project will be able to import the project directly from a build dir:
configure_file(${CMAKE_SOURCE_DIR}/cmake/RobotDriverConfig.cmake RobotDriverConfig.cmake COPYONLY)
export(EXPORT RobotDriverTargets
  NAMESPACE RobotDriver::
  FILE "${PROJECT_BINARY_DIR}/RobotDriverTargets.cmake"
)

# The file we created earlier:
install(FILES ${CMAKE_SOURCE_DIR}/cmake/RobotDriverConfig.cmake
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/RobotDriver"
)


###############################################################################
# Pack project
###############################################################################
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Lazyshawn")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_SOURCE_GENERATOR "ZIP")

include(CPack)
