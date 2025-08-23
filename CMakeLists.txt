# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 4.0)

project(MeddySDK_Meddyproject
  VERSION 0.1.0
  DESCRIPTION ""
  HOMEPAGE_URL "https://github.com/ChristianHinko/MeddySDK_Meddyproject"
  LANGUAGES CXX
  )

# Tell CMake where our cmake scripts ("modules") are.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/CPack")

# Set up testing. Defines `BUILD_TESTING` which is ON by default. This must be called by the top-level project only.
if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
endif()

#
# We want to be build interface users of our subprojects instead of install
# interface users, i.e., we want to build with our subprojects instead of building off
# of pre-built binaries of them. This is how the development environment should be. We
# want to build and develop all of these subprojects together.
#

include(FetchContent)

#
# Declare subprojects.
#
# We use `FetchContent_Declare()` to allow us to build with our subprojects instead
# of building off of intalled packages of them.
#
# The `find_package()` command is what our projects use to retrieve external dependencies
# and its default behavior looks through installed files to get them. So we use `OVERRIDE_FIND_PACKAGE`
# to have it look to FetchContent declarations instead.
#
# Our subprojects exist as subdirectories so we just have their FetchContent
# declarations refer to them by directory path.
#

FetchContent_Declare(MeddySDK_Meddyproject_Include
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Include"
  )

FetchContent_Declare(MeddySDK_Meddyproject_Source
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source"
  )

FetchContent_Declare(MeddySDK_Meddyproject_Static
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Static"
  )

FetchContent_Declare(MeddySDK_Meddyproject_Shared
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Shared"
  )

FetchContent_Declare(MeddySDK_Meddyproject_Object
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Object"
  )

if(BUILD_TESTING)
  FetchContent_Declare(MeddySDK_Meddyproject_Tests
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Tests"
    )
endif()

#
# Add subprojects.
#
# We use `FetchContent_MakeAvailable()` because it avoids project duplication for
# us. If we were to use `add_subdirectory()` ourselves directly, we would have to set up
# guards against adding subdirectories multiple times. That's prone to happening
# when a project is needed as a dependency several times.
#

FetchContent_MakeAvailable(MeddySDK_Meddyproject_Include)

FetchContent_MakeAvailable(MeddySDK_Meddyproject_Source)

FetchContent_MakeAvailable(MeddySDK_Meddyproject_Static)

FetchContent_MakeAvailable(MeddySDK_Meddyproject_Shared)

FetchContent_MakeAvailable(MeddySDK_Meddyproject_Object)

# Conditionally enable our "tests" subproject.
if(BUILD_TESTING)
  FetchContent_MakeAvailable(MeddySDK_Meddyproject_Tests)
endif()

include(GNUInstallDirs)

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
