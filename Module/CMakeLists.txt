# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 4.0)

project(MeddySDK_Meddyproject_Module
  VERSION 0.1.0
  LANGUAGES CXX
  )

#
# Create targets.
#
# We namespace target names using underscore characters "_". Namespacing helps avoid ambiguity. E.g., for
# the DAM library of MeddySDK, we name it "MeddySDK_DAM" instead of just "DAM", because that would be very vague.
#

add_library(MeddySDK_Meddyproject_Module MODULE)

#
# Set output names of our targets.
#
# This determines the name of the binary files generated for the target. The name may differ from the actual
# target name. E.g., for MeddySDKCLI, we name its executable file "meddy" to suit our desired command line usage.
#

set_target_properties(MeddySDK_Meddyproject_Module
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

add_library(MeddySDK::Meddyproject::Module ALIAS MeddySDK_Meddyproject_Module)

# Generate a cpp header file that provides export macros for our library.
include(GenerateExportHeader)
generate_export_header(MeddySDK_Meddyproject_Module
  BASE_NAME MeddySDK_Meddyproject
  EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/Include/meddysdk_meddyproject_module_export.h"
  INCLUDE_GUARD_NAME "MEDDYSDK_MEDDYPROJECT_MODULE_EXPORT_H"
  )

# List out required language features.
target_compile_features(MeddySDK_Meddyproject_Module
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
# Interface and/or public file sets are important because we use them to install headers for
# the install interface users.
#
# A nice benefit of listing all header files is that they can be easily-discoverable by IDEs.
#
target_sources(MeddySDK_Meddyproject_Module
  PUBLIC
    FILE_SET meddysdk_meddyproject_module_public_headers
      TYPE HEADERS
      BASE_DIRS
        "${CMAKE_CURRENT_BINARY_DIR}/Include" # The base directory of generated headers.
      FILES
        "${CMAKE_CURRENT_BINARY_DIR}/Include/meddysdk_meddyproject_module_export.h"
  )

# Note that we do not "find package" for our parent project. We don't need to since we are built in the same
# cmake invocation as the it. That means we're being processed during the same configuration step as them, which
# means we'll have all their targets. Also, the targets that we reference in `target_link_libraries` commands don't
# even need to exist yet because CMake only validates the existence of targets at the end of the entire
# configuration process.

# Add dependencies to other targets. For imported targets, it's important to only depend on them if we
# are BUILD_LOCAL_INTERFACE, because we don't want them to get mixed into our export.
target_link_libraries(MeddySDK_Meddyproject_Module
  INTERFACE
    $<BUILD_LOCAL_INTERFACE:MeddySDK::Meddyproject::Include>
  PRIVATE
    $<BUILD_LOCAL_INTERFACE:MeddySDK::Meddyproject::Source>
  )

include(GNUInstallDirs)

# Associate our targets with an export and specify how their files would be installed.
install(TARGETS MeddySDK_Meddyproject_Module
  EXPORT "MeddySDK_MeddyprojectExport"
  COMPONENT "Module"
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
  FILE_SET meddysdk_meddyproject_module_public_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
  )
