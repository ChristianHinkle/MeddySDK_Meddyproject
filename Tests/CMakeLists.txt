# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 4.0)

project(MeddySDK_Meddyproject_Tests
  VERSION 0.1.0
  LANGUAGES CXX
  )

#
# Get the MeddySDK_Meddyproject_Static export. This gives us its imported targets.
#
# TODO: Remove the "_Static" postfix and use `COMPONENT` to access the static library. We can't right now because we
# are in an intermediate point of transitioning to this new organized structure. So "MeddySDK_Meddyproject_Static" is THE export, for now.
#
find_package(MeddySDK_Meddyproject_Static 0 CONFIG REQUIRED)

add_executable(ProjectRootToDotMeddyprojectPath)
target_compile_features(ProjectRootToDotMeddyprojectPath PUBLIC cxx_std_20)
target_sources(ProjectRootToDotMeddyprojectPath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/ProjectRootToDotMeddyprojectPath.cpp")
target_link_libraries(ProjectRootToDotMeddyprojectPath PRIVATE MeddySDK::Meddyproject::Static)

add_test(NAME MeddySDK.Meddyproject.ProjectRootToDotMeddyprojectPath.UserVideos
  COMMAND ProjectRootToDotMeddyprojectPath
    "C:/Users/Meddy Man/Videos"
    "C:/Users/Meddy Man/Videos/.meddyproject"
  )

add_test(NAME MeddySDK.Meddyproject.ProjectRootToDotMeddyprojectPath.RootC
  COMMAND ProjectRootToDotMeddyprojectPath
    "C:/"
    "C:/.meddyproject"
  )

add_test(NAME MeddySDK.Meddyproject.ProjectRootToDotMeddyprojectPath.Empty
  COMMAND ProjectRootToDotMeddyprojectPath
    ""
    ".meddyproject"
  )

add_executable(DotMeddyprojectToProjectRootPath)
target_compile_features(DotMeddyprojectToProjectRootPath PUBLIC cxx_std_20)
target_sources(DotMeddyprojectToProjectRootPath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/DotMeddyprojectToProjectRootPath.cpp")
target_link_libraries(DotMeddyprojectToProjectRootPath PRIVATE MeddySDK::Meddyproject::Static)

add_test(NAME MeddySDK.Meddyproject.DotMeddyprojectToProjectRootPath.UserVideos
  COMMAND DotMeddyprojectToProjectRootPath
    "C:/Users/Meddy Man/Videos/.meddyproject"
    "C:/Users/Meddy Man/Videos"
  )

add_test(NAME MeddySDK.Meddyproject.DotMeddyprojectToProjectRootPath.RootC
  COMMAND DotMeddyprojectToProjectRootPath
    "C:/.meddyproject"
    "C:/"
  )

add_test(NAME MeddySDK.Meddyproject.DotMeddyprojectToProjectRootPath.Empty
  COMMAND DotMeddyprojectToProjectRootPath
    ".meddyproject"
    ""
  )

add_executable(DotMeddyprojectToManifestFilePath)
target_compile_features(DotMeddyprojectToManifestFilePath PUBLIC cxx_std_20)
target_sources(DotMeddyprojectToManifestFilePath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/DotMeddyprojectToManifestFilePath.cpp")
target_link_libraries(DotMeddyprojectToManifestFilePath PRIVATE MeddySDK::Meddyproject::Static)

add_test(NAME MeddySDK.Meddyproject.DotMeddyprojectToManifestFilePath.UserVideos
  COMMAND DotMeddyprojectToManifestFilePath
    "C:/Users/Meddy Man/Videos/.meddyproject"
    "C:/Users/Meddy Man/Videos/.meddyproject/Manifest.json"
  )

add_test(NAME MeddySDK.Meddyproject.DotMeddyprojectToManifestFilePath.RootC
  COMMAND DotMeddyprojectToManifestFilePath
    "C:/.meddyproject"
    "C:/.meddyproject/Manifest.json"
  )

add_test(NAME MeddySDK.Meddyproject.DotMeddyprojectToManifestFilePath.Empty
  COMMAND DotMeddyprojectToManifestFilePath
    ".meddyproject"
    ".meddyproject/Manifest.json"
  )
