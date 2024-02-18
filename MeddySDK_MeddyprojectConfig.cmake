
include(CMakeFindDependencyMacro)

# Get the Boost export.
find_dependency(Boost CONFIG COMPONENTS filesystem)

# Include our export. This imports all of our targets.
include("${CMAKE_CURRENT_LIST_DIR}/MeddySDK_MeddyprojectExport.cmake")
