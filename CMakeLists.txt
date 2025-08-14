# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 4.0)

project(MeddySDK_Meddyproject
  VERSION 0.1.0
  DESCRIPTION ""
  HOMEPAGE_URL "https://github.com/ChristianHinko/MeddySDK_Meddyproject"
  LANGUAGES CXX
  )

# Set up testing. Defines `BUILD_TESTING` which is ON by default. This must be called by the top-level project only.
if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
endif()

# Conditionally enable our "tests" subproject.
if(BUILD_TESTING)
  # The reason we're okay with using `add_subdirectory` for this instead of a more-sophisticated `FetchContent` setup is
  # because we're fairly certain that we are the only project that would want to use this testing version of our project. If we
  # want to change this, we could structure both the test and non-test version of this project into two different subdirectories,
  # which would be tied together by `FetchContent` from a parent project. This would allow one to find the other in a
  # decoupled manner.
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Tests")
endif()

# Tell CMake where our cmake scripts ("modules") are.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/CPack")

#
# Create targets.
#
# We namespace target names using underscore characters "_". Namespacing helps avoid ambiguity. E.g., for
# the DAM library of MeddySDK, we name it "MeddySDK_DAM" instead of just "DAM", because that would be very vague.
#

add_library(MeddySDK_Meddyproject)

#
# Set output names of our targets.
#
# This determines the name of the binary files generated for the target. The name may differ from the actual
# target name. E.g., for MeddySDKCLI, we name its executable file "meddy" to suit our desired command line usage.
#

set_target_properties(MeddySDK_Meddyproject
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

add_library(MeddySDK::Meddyproject ALIAS MeddySDK_Meddyproject)

# Generate a cpp header file that provides export macros for our library.
include(GenerateExportHeader)
generate_export_header(MeddySDK_Meddyproject)

# List out required language features.
target_compile_features(MeddySDK_Meddyproject
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
target_sources(MeddySDK_Meddyproject
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
target_sources(MeddySDK_Meddyproject
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/Meddyproject.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/FilesystemUtils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/StringUtils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/CharBufferString.cpp"
  )

include(GNUInstallDirs)

# Associate our targets with an export and specify how their files would be installed.
install(TARGETS MeddySDK_Meddyproject
  EXPORT "${PROJECT_NAME}Export"
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
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
  # Copy over public headers.
  FILE_SET meddysdk_meddyproject_public_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
  )

# Generate script that imports targets associated with our export.
install(EXPORT "${PROJECT_NAME}Export"
  FILE "${PROJECT_NAME}Export.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )

# Create a package version file for our export.
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
  )

# Copy any files for downstream usage to the install directory.
install(
  FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )

# Set some simple values for CPack based on this current project. The default
# values for these are specific to the top-level project but we want to allow for
# having several projects generate their own CPack configs without conflicting.
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};${PROJECT_NAME};ALL;/")

# Tell CMake to output the generated CPack configs in a directory specific to this project. This is
# nice so we can generate our own config without conflicting with other projects.
set(CPACK_OUTPUT_CONFIG_FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/CPackConfig.cmake")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/CPackSourceConfig.cmake")

# TODO: Decide on an appropriate place to set generator-specific variables. I tried
# wrapping this NSIS-specific variable in an if condition `CPACK_GENERATOR MATCHES "NSIS"` but
# it was never being entered.

#
# Use NSIS' feature for appending our binary directory to the PATH environment
# variable.
#
# Important: Make sure you modify your NSIS installation with the files from the "strlen_8192" version before
# running CPack! Or else the installer will fail to modify the PATH env var for most users, because
# variables in NSIS cannot exceed 1024 characters by default, and PATH env vars tend to be very lengthy.
#
# See our custom NSIS template where we make sure the "PATH" environment variable is
# modified by default: "CMake/Modules/CPack/NSIS.InstallOptions.ini.in".
#
# TODO: I would like to have this functionality for other generators as well, but I
# can't seem to find a generator-agnostic solution to this. E.g., in CMake's source
# code, they re-implement this feature specifically for their WIX generator. I believe this
# kind of per-generator implementation would be the "correct" way. Alternatively, it might
# be possible to invoke a script that sets the env var manually after the generator finishes
# installation of the package, but I would want to be transparent about that and give the
# user control over that behavior; not to mention, it would also have to do the opposite
# on uninstall.
#
set(CPACK_NSIS_MODIFY_PATH "ON")

# Generate a CPackConfig.cmake file for this project.
include(CPack)
