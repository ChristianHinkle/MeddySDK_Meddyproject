# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 4.0)

project(MeddySDK_Meddyproject_Static
  VERSION 0.1.0
  LANGUAGES CXX
  )

#
# Create targets.
#
# We namespace target names using underscore characters "_". Namespacing helps avoid ambiguity. E.g., for
# the DAM library of MeddySDK, we name it "MeddySDK_DAM" instead of just "DAM", because that would be very vague.
#

add_library(MeddySDK_Meddyproject_Static)

#
# Set output names of our targets.
#
# This determines the name of the binary files generated for the target. The name may differ from the actual
# target name. E.g., for MeddySDKCLI, we name its executable file "meddy" to suit our desired command line usage.
#

set_target_properties(MeddySDK_Meddyproject_Static
  PROPERTIES
    OUTPUT_NAME "MeddySDK_Meddyproject"
  )

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

add_library(MeddySDK::Meddyproject::Static ALIAS MeddySDK_Meddyproject_Static)

# Generate a cpp header file that provides export macros for our library.
include(GenerateExportHeader)
generate_export_header(MeddySDK_Meddyproject_Static
  BASE_NAME MeddySDK_Meddyproject
  )

# List out required language features.
target_compile_features(MeddySDK_Meddyproject_Static
  PUBLIC
    cxx_std_20
  )

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
target_sources(MeddySDK_Meddyproject_Static
  PUBLIC
    FILE_SET meddysdk_meddyproject_public_headers
      TYPE HEADERS
      BASE_DIRS
        # Base directory of public headers.
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Public"
        # Base directory of generated headers.
        "${CMAKE_CURRENT_BINARY_DIR}"
      FILES
        # Add all public header files.
        "${CMAKE_CURRENT_BINARY_DIR}/meddysdk_meddyproject_export.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Public/Meddyproject.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Public/FilesystemUtils.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Public/StringUtils.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Public/CharBufferString.h"
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
        "${CMAKE_CURRENT_SOURCE_DIR}/Source"
      FILES
        # Add all private header files.
  )

# Add all source files.
target_sources(MeddySDK_Meddyproject_Static
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/Meddyproject.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/FilesystemUtils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/StringUtils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/CharBufferString.cpp"
  )

include(GNUInstallDirs)

# Associate our targets with an export and specify how their files would be installed.
install(TARGETS MeddySDK_Meddyproject_Static
  EXPORT "MeddySDK_MeddyprojectExport"
  COMPONENT "Static"
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
