# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 4.0)

project(MeddySDK_Meddyproject_Tests
  VERSION 0.1.0
  LANGUAGES CXX
  )

# Note that we do not "find package" for our parent project. We don't need to since we are built in the same
# cmake invocation as the it. That means we're being processed during the same configuration step as them, which
# means we'll have all their targets. Also, the targets that we reference in `target_link_libraries` commands don't
# even need to exist yet because CMake only validates the existence of targets at the end of the entire
# configuration process.

add_executable(ProjectRootToDotMeddyprojectPath)
target_compile_features(ProjectRootToDotMeddyprojectPath PUBLIC cxx_std_20)
target_sources(ProjectRootToDotMeddyprojectPath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/ProjectRootToDotMeddyprojectPath.cpp")
target_link_libraries(ProjectRootToDotMeddyprojectPath PRIVATE MeddySDK::Meddyproject::Static)

add_test(NAME MeddySDK.Meddyproject.ProjectRootToDotMeddyprojectPath.UserVideos
  COMMAND ProjectRootToDotMeddyprojectPath
    "/Users/Meddy Man/Videos"
    "/Users/Meddy Man/Videos/.meddyproject"
  )

add_test(NAME MeddySDK.Meddyproject.ProjectRootToDotMeddyprojectPath.RootC
  COMMAND ProjectRootToDotMeddyprojectPath
    "/"
    "/.meddyproject"
  )

add_test(NAME MeddySDK.Meddyproject.ProjectRootToDotMeddyprojectPath.Empty
  COMMAND ProjectRootToDotMeddyprojectPath
    ""
    ".meddyproject"
  )

add_executable(DotMeddyprojectToProjectRootPath)
target_compile_features(DotMeddyprojectToProjectRootPath PUBLIC cxx_std_20)
target_sources(DotMeddyprojectToProjectRootPath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/DotMeddyprojectToProjectRootPath.cpp")
target_link_libraries(DotMeddyprojectToProjectRootPath PRIVATE MeddySDK::Meddyproject::Static)

add_test(NAME MeddySDK.Meddyproject.DotMeddyprojectToProjectRootPath.UserVideos
  COMMAND DotMeddyprojectToProjectRootPath
    "/Users/Meddy Man/Videos/.meddyproject"
    "/Users/Meddy Man/Videos"
  )

add_test(NAME MeddySDK.Meddyproject.DotMeddyprojectToProjectRootPath.RootC
  COMMAND DotMeddyprojectToProjectRootPath
    "/.meddyproject"
    "/"
  )

add_test(NAME MeddySDK.Meddyproject.DotMeddyprojectToProjectRootPath.Empty
  COMMAND DotMeddyprojectToProjectRootPath
    ".meddyproject"
    ""
  )

add_executable(DotMeddyprojectToManifestFilePath)
target_compile_features(DotMeddyprojectToManifestFilePath PUBLIC cxx_std_20)
target_sources(DotMeddyprojectToManifestFilePath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/DotMeddyprojectToManifestFilePath.cpp")
target_link_libraries(DotMeddyprojectToManifestFilePath PRIVATE MeddySDK::Meddyproject::Static)

add_test(NAME MeddySDK.Meddyproject.DotMeddyprojectToManifestFilePath.UserVideos
  COMMAND DotMeddyprojectToManifestFilePath
    "/Users/Meddy Man/Videos/.meddyproject"
    "/Users/Meddy Man/Videos/.meddyproject/Manifest.json"
  )

add_test(NAME MeddySDK.Meddyproject.DotMeddyprojectToManifestFilePath.RootC
  COMMAND DotMeddyprojectToManifestFilePath
    "/.meddyproject"
    "/.meddyproject/Manifest.json"
  )

add_test(NAME MeddySDK.Meddyproject.DotMeddyprojectToManifestFilePath.Empty
  COMMAND DotMeddyprojectToManifestFilePath
    ".meddyproject"
    ".meddyproject/Manifest.json"
  )

add_executable(TryCreateNewProject)
target_compile_features(TryCreateNewProject PUBLIC cxx_std_20)
target_sources(TryCreateNewProject PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/TryCreateNewProject.cpp")
target_link_libraries(TryCreateNewProject PRIVATE MeddySDK::Meddyproject::Static)

add_test(
  NAME MeddySDK.Meddyproject.TryCreateNewProject.SuccessCase
  COMMAND ${CMAKE_COMMAND}
    -D"CMAKE_MODULE_PATH"=${CMAKE_MODULE_PATH}
    -D"MY_TEST_EXECUTABLE_PATH"='$<TARGET_FILE:TryCreateNewProject>'
    -D"MY_TEST_WORKING_DIRECTORY"='${CMAKE_CURRENT_BINARY_DIR}'
    -P "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Scripts/TryCreateNewProject_SuccessCase.cmake"
  )
