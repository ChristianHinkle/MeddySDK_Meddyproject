
cmake_minimum_required(VERSION 3.8)

project(Meddyproject
  VERSION 0.1
  LANGUAGES CXX
  )

add_library(Meddyproject)

# Add source files. Including header files as well so they are easily-discoverable by IDEs.
target_sources(Meddyproject
  PUBLIC
    # Add all public headers under a file set.
    #
    # This file set is usable for installing headers.
    FILE_SET meddyproject_public_headers
      TYPE HEADERS
      BASE_DIRS
        "${Meddyproject_SOURCE_DIR}/Source/Public"
        "${Meddyproject_BINARY_DIR}" # base directory of generated headers
      FILES
        # Add all public header files.
        "${Meddyproject_SOURCE_DIR}/Source/Public/Meddyproject.h"
  PRIVATE
    # Add all private source files.
    "${Meddyproject_SOURCE_DIR}/Source/Private/Meddyproject.cpp"
  )

# Add include directories.
#
# All are enclosed in BUILD_INTERFACE because they are specific to our source directory and
# should be used only when building binaries for our target.
#
# No install-specific directories are provided here. That gets taken care of
# elsewhere, i.e., the install command.
target_include_directories(Meddyproject
  PUBLIC
    "$<BUILD_INTERFACE:${Meddyproject_SOURCE_DIR}/Source/Public>" # add our public includes
    "$<BUILD_INTERFACE:${Meddyproject_BINARY_DIR}>" # add easy access to generated headers
  PRIVATE
    "$<BUILD_INTERFACE:${Meddyproject_SOURCE_DIR}/Source>" # add easy access to non-public includes
  )

include(GNUInstallDirs)

# Associate our target with an export and set up installation.
install(TARGETS Meddyproject
  EXPORT MeddyprojectExport
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" # gets added to interface include directories
  FILE_SET meddyproject_public_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Meddyproject" # copy over public headers
  )

# Generate script that imports targets associated with our export.
install(EXPORT MeddyprojectExport
  FILE "MeddyprojectExport.cmake"
  NAMESPACE MeddySDK::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Meddyproject"
  )

# Create a package version file for our export.
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/MeddyprojectConfigVersion.cmake"
  VERSION ${Meddyproject_VERSION}
  COMPATIBILITY SameMajorVersion
  )

# Copy any files for downstream usage to the install directory.
install(
  FILES
    "${Meddyproject_SOURCE_DIR}/MeddyprojectConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MeddyprojectConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Meddyproject"
  )

# Enable C++20 if supported.
if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Meddyproject PROPERTY CXX_STANDARD 20)
endif()

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
