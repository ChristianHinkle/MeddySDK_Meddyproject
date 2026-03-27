# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 4.0)

# Tell CMake where our cmake scripts ("modules") are.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeProjectFramework")

include(CMakeProjectFramework)

cmakeprojectframework_cmakelists_mainproject_library("MeddySDK_Meddyproject" "MeddySDK" "Meddyproject")
