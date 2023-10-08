
cmake_minimum_required(VERSION 3.8)

project(MeddySDK_Meddyproject
  VERSION 0.1
  LANGUAGES CXX
  )

add_library(MeddySDK_Meddyproject)

# Add source files. Including header files as well so they are easily-discoverable by IDEs.
target_sources(MeddySDK_Meddyproject
  PUBLIC
    # Add all public headers under a file set.
    #
    # This file set is usable for installing headers.
    FILE_SET meddysdk_meddyproject_public_headers
      TYPE HEADERS
      BASE_DIRS
        "${MeddySDK_Meddyproject_SOURCE_DIR}/Source/Public"
        "${MeddySDK_Meddyproject_BINARY_DIR}" # base directory of generated headers
      FILES
        # Add all public header files.
        "${MeddySDK_Meddyproject_SOURCE_DIR}/Source/Public/Meddyproject.h"
  PRIVATE
    # Add all private source files.
    "${MeddySDK_Meddyproject_SOURCE_DIR}/Source/Private/Meddyproject.cpp"
  )

# Add include directories.
#
# All are enclosed in BUILD_INTERFACE because they are specific to our source directory and
# should be used only when building binaries for our target.
#
# No install-specific directories are provided here. That gets taken care of
# elsewhere, i.e., the install command.
#
# Also, make sure include statements of public headers are compatible between install interface users
# and build interface users. E.g., a user's include statements should work for both packaged
# library usage and subproject library usage, without having to adapt their code to either usage.
target_include_directories(MeddySDK_Meddyproject
  PUBLIC
    "$<BUILD_INTERFACE:${MeddySDK_Meddyproject_SOURCE_DIR}/Source/Public>" # add our public includes
    "$<BUILD_INTERFACE:${MeddySDK_Meddyproject_BINARY_DIR}>" # add easy access to generated headers
  PRIVATE
    "$<BUILD_INTERFACE:${MeddySDK_Meddyproject_SOURCE_DIR}/Source>" # add easy access to non-public includes
  )

include(GNUInstallDirs)

# Associate our target with an export and set up installation.
install(TARGETS MeddySDK_Meddyproject
  EXPORT MeddySDK_MeddyprojectExport
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MeddySDK_Meddyproject" # gets added to interface include directories
  FILE_SET meddysdk_meddyproject_public_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MeddySDK_Meddyproject" # copy over public headers
  )

# Generate script that imports targets associated with our export.
install(EXPORT MeddySDK_MeddyprojectExport
  FILE "MeddySDK_MeddyprojectExport.cmake"
  NAMESPACE MeddySDK_MeddyprojectExport::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/MeddySDK_Meddyproject"
  )

# Create a package version file for our export.
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/MeddySDK_MeddyprojectConfigVersion.cmake"
  VERSION ${MeddySDK_Meddyproject_VERSION}
  COMPATIBILITY SameMajorVersion
  )

# Copy any files for downstream usage to the install directory.
install(
  FILES
    "${MeddySDK_Meddyproject_SOURCE_DIR}/MeddySDK_MeddyprojectConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MeddySDK_MeddyprojectConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/MeddySDK_Meddyproject"
  )
