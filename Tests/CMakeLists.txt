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

add_executable(MeddySDK_Meddyproject_Test_ProjectRootToDotMeddyprojectPath)
target_compile_features(MeddySDK_Meddyproject_Test_ProjectRootToDotMeddyprojectPath PUBLIC cxx_std_20)
target_sources(MeddySDK_Meddyproject_Test_ProjectRootToDotMeddyprojectPath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/ProjectRootToDotMeddyprojectPath.cpp")
target_link_libraries(MeddySDK_Meddyproject_Test_ProjectRootToDotMeddyprojectPath PRIVATE MeddySDK::Meddyproject)

add_test(NAME MeddySDK.Meddyproject.Test.ProjectRootToDotMeddyprojectPath.UserVideos
  COMMAND MeddySDK_Meddyproject_Test_ProjectRootToDotMeddyprojectPath
    "C:/Users/Meddy Man/Videos"
    "C:/Users/Meddy Man/Videos/.meddyproject"
  )

add_test(NAME MeddySDK.Meddyproject.Test.ProjectRootToDotMeddyprojectPath.RootC
  COMMAND MeddySDK_Meddyproject_Test_ProjectRootToDotMeddyprojectPath
    "C:/"
    "C:/.meddyproject"
  )

add_test(NAME MeddySDK.Meddyproject.Test.ProjectRootToDotMeddyprojectPath.Empty
  COMMAND MeddySDK_Meddyproject_Test_ProjectRootToDotMeddyprojectPath
    ""
    ".meddyproject"
  )

add_executable(MeddySDK_Meddyproject_Test_DotMeddyprojectToProjectRootPath)
target_compile_features(MeddySDK_Meddyproject_Test_DotMeddyprojectToProjectRootPath PUBLIC cxx_std_20)
target_sources(MeddySDK_Meddyproject_Test_DotMeddyprojectToProjectRootPath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/DotMeddyprojectToProjectRootPath.cpp")
target_link_libraries(MeddySDK_Meddyproject_Test_DotMeddyprojectToProjectRootPath PRIVATE MeddySDK::Meddyproject)

add_test(NAME MeddySDK.Meddyproject.Test.DotMeddyprojectToProjectRootPath.UserVideos
  COMMAND MeddySDK_Meddyproject_Test_DotMeddyprojectToProjectRootPath
    "C:/Users/Meddy Man/Videos/.meddyproject"
    "C:/Users/Meddy Man/Videos"
  )

add_test(NAME MeddySDK.Meddyproject.Test.DotMeddyprojectToProjectRootPath.RootC
  COMMAND MeddySDK_Meddyproject_Test_DotMeddyprojectToProjectRootPath
    "C:/.meddyproject"
    "C:/"
  )

add_test(NAME MeddySDK.Meddyproject.Test.DotMeddyprojectToProjectRootPath.Empty
  COMMAND MeddySDK_Meddyproject_Test_DotMeddyprojectToProjectRootPath
    ".meddyproject"
    ""
  )

add_executable(MeddySDK_Meddyproject_Test_DotMeddyprojectToManifestFilePath)
target_compile_features(MeddySDK_Meddyproject_Test_DotMeddyprojectToManifestFilePath PUBLIC cxx_std_20)
target_sources(MeddySDK_Meddyproject_Test_DotMeddyprojectToManifestFilePath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/DotMeddyprojectToManifestFilePath.cpp")
target_link_libraries(MeddySDK_Meddyproject_Test_DotMeddyprojectToManifestFilePath PRIVATE MeddySDK::Meddyproject)

add_test(NAME MeddySDK.Meddyproject.Test.DotMeddyprojectToManifestFilePath.UserVideos
  COMMAND MeddySDK_Meddyproject_Test_DotMeddyprojectToManifestFilePath
    "C:/Users/Meddy Man/Videos/.meddyproject"
    "C:/Users/Meddy Man/Videos/.meddyproject/Manifest.json"
  )

add_test(NAME MeddySDK.Meddyproject.Test.DotMeddyprojectToManifestFilePath.RootC
  COMMAND MeddySDK_Meddyproject_Test_DotMeddyprojectToManifestFilePath
    "C:/.meddyproject"
    "C:/.meddyproject/Manifest.json"
  )

add_test(NAME MeddySDK.Meddyproject.Test.DotMeddyprojectToManifestFilePath.Empty
  COMMAND MeddySDK_Meddyproject_Test_DotMeddyprojectToManifestFilePath
    ".meddyproject"
    ".meddyproject/Manifest.json"
  )
