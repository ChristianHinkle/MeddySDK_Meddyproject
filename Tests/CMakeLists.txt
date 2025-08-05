# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 4.0)

project(MeddySDK_Meddyproject_Tests
  VERSION 0.1.0
  LANGUAGES CXX
  )

# Tell CMake where our cmake scripts ("modules") are.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/CPack")

# Get the MeddySDK_Meddyproject export. This gives us its imported targets.
find_package(MeddySDK_Meddyproject 0 CONFIG REQUIRED)

add_executable(MeddySDK_Meddyproject_Test_Path_MakeProjectPath)
target_compile_features(MeddySDK_Meddyproject_Test_Path_MakeProjectPath PUBLIC cxx_std_20)
target_sources(MeddySDK_Meddyproject_Test_Path_MakeProjectPath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/Path_MakeProjectPath.cpp")
target_link_libraries(MeddySDK_Meddyproject_Test_Path_MakeProjectPath PRIVATE MeddySDK::Meddyproject)

add_test(NAME MeddySDK.Meddyproject.Test.Path.MakeProjectPath.UserVideos
  COMMAND MeddySDK_Meddyproject_Test_Path_MakeProjectPath
    "C:/Users/Meddy Man/Videos"
    "C:/Users/Meddy Man/Videos/.meddyproject"
  )

add_test(NAME MeddySDK.Meddyproject.Test.Path.MakeProjectPath.RootC
  COMMAND MeddySDK_Meddyproject_Test_Path_MakeProjectPath
    "C:/"
    "C:/.meddyproject"
  )

add_test(NAME MeddySDK.Meddyproject.Test.Path.MakeProjectPath.Empty
  COMMAND MeddySDK_Meddyproject_Test_Path_MakeProjectPath
    ""
    ".meddyproject"
  )
