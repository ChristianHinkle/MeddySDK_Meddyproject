# Copyright (c) 2023-2024 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 3.24)

project(MeddySDK_Meddyproject
  VERSION 0.1
  LANGUAGES CXX
  )

#
# Add targets.
#
# We namespace target names using underscore characters "_". Not namespacing your target names can result in their
# file names not be specific enough. E.g., for the DAM library of MeddySDK, we want its library file to be named
# something like "MeddySDK_DAM.lib" instead of just "DAM.lib" - because that would be super vague.
#

add_library(MeddySDK_Meddyproject)

#
# Add alias targets.
#
# We name these with special characters that filenames can't have. We use these aliases instead of the direct
# target names where possible so that they can't be be interpreted as filenames.
#
# It's conventional to use "::" in these names, like namespaces. If a target doesn't have anything to namespace in its
# name then just prepend "::" to it, analogous to how we indicate global scope in cpp.
#
# The aliases set up here get used by build interface users. Make sure the same aliases are also set up for install
# interface users somewhere else. We do that in the export's config file. It's important to make sure the names are
# the same in both cases for consistency. Namely, so that build interface users can use `OVERRIDE_FIND_PACKAGE` with
# `FetchContent_Declare()`.
#

add_library(MeddySDK::Meddyproject ALIAS MeddySDK_Meddyproject)

# List out required language features.
target_compile_features(MeddySDK_Meddyproject
  PUBLIC
    cxx_std_20
  )

# Generate cpp header file with export macros for our target.
include(GenerateExportHeader)
generate_export_header(MeddySDK_Meddyproject)

#
# Add all header files and set up their include directories.
#
# The `BASE_DIRS` of file sets automatically get added to the target's include
# directories. So we don't use the `target_include_directories()` command anymore
# because that would be redundant.
#
# The file set of public headers is important because we use it to install
# them for install interface users. It's very important to list out all public headers
# in this file set so that install interface users have them.
#
# A nice benefit of listing all header files is that they can be easily-discoverable by IDEs.
#
target_sources(MeddySDK_Meddyproject
  PUBLIC
    FILE_SET meddysdk_meddyproject_public_headers
      TYPE HEADERS
      BASE_DIRS
        # Base directory of public headers.
        "${MeddySDK_Meddyproject_SOURCE_DIR}/Source/Public"
        # Base directory of generated headers.
        "${CMAKE_CURRENT_BINARY_DIR}"
      FILES
        # Add all public header files.
        "${CMAKE_CURRENT_BINARY_DIR}/meddysdk_meddyproject_export.h"
        "${MeddySDK_Meddyproject_SOURCE_DIR}/Source/Public/ProjectPath.h"
  PRIVATE
    FILE_SET meddysdk_meddyproject_private_headers
      TYPE HEADERS
      BASE_DIRS
        #
        # Base directory of private headers.
        #
        # It's nice to leave the "Private" directory out of this base include path so that, for
        # cpp include statements, you have to type "Private/" to access the non-public headers. This
        # makes it obvious for programmers when a header shouldn't be included in public files.
        #
        "${MeddySDK_Meddyproject_SOURCE_DIR}/Source"
      FILES
        # Add all private header files.
  )

# Add all source files.
target_sources(MeddySDK_Meddyproject
  PRIVATE
    "${MeddySDK_Meddyproject_SOURCE_DIR}/Source/Private/ProjectPath.cpp"
  )

include(GNUInstallDirs)

# Associate our target with an export and set up installation.
install(TARGETS MeddySDK_Meddyproject
  EXPORT MeddySDK_MeddyprojectExport
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  #
  # Add to interface include directories.
  #
  # Note: We take care of install-specific include directories here instead of `target_include_directories()`.
  #
  # Note: It's important to make sure cpp include statements of public headers are compatible between
  # install interface users and build interface users. E.g., a user's include statements should work for
  # both packaged library usage and subproject library usage, without having to adapt their code to either use case.
  #
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MeddySDK_Meddyproject"
  # Copy over public headers.
  FILE_SET meddysdk_meddyproject_public_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MeddySDK_Meddyproject"
  )

# Generate script that imports targets associated with our export.
install(EXPORT MeddySDK_MeddyprojectExport
  FILE "MeddySDK_MeddyprojectExport.cmake"
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
