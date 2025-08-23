# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

include(CMakeFindDependencyMacro)

# Include our export. This imports all of our targets.
include("${CMAKE_CURRENT_LIST_DIR}/MeddySDK_MeddyprojectExport.cmake")

#
# Add alias targets whose names match those from the project file.
#
# It's important to make sure the names are the same for consistency. Namely, so that build
# interface users can use `OVERRIDE_FIND_PACKAGE` with `FetchContent_Declare()`.
#

add_library(MeddySDK::Meddyproject::Include ALIAS MeddySDK_Meddyproject_Include)
add_library(MeddySDK::Meddyproject::Static ALIAS MeddySDK_Meddyproject_Static)
add_library(MeddySDK::Meddyproject::Shared ALIAS MeddySDK_Meddyproject_Shared)
add_library(MeddySDK::Meddyproject::Module ALIAS MeddySDK_Meddyproject_Module)
add_library(MeddySDK::Meddyproject::Object ALIAS MeddySDK_Meddyproject_Object)
