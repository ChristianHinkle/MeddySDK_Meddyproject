# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 4.0)

# Write out the main project name here, in one place, to make renaming easier. This is mainly used by our
# boilerplate code. There may be other mentions of the project's name mixed in certain places of this file, so
# don't rely on these variables alone for renaming.
SET(MY_BASE_PROJECT_NAME_FULL "MeddySDK_Meddyproject")
string(TOLOWER ${MY_BASE_PROJECT_NAME_FULL} MY_BASE_PROJECT_NAME_FULL_LOWERCASE)
string(TOUPPER ${MY_BASE_PROJECT_NAME_FULL} MY_BASE_PROJECT_NAME_FULL_UPPERCASE)
SET(MY_BASE_PROJECT_NAME_NAMESPACE "MeddySDK")
SET(MY_BASE_PROJECT_NAME_LEAFNAME "Meddyproject")

project(${MY_BASE_PROJECT_NAME_FULL}_Tests
  VERSION 0.1.0
  LANGUAGES CXX
  )

find_package(Boost 0 CONFIG REQUIRED COMPONENTS filesystem)

# Note that we do not "find package" for our parent project. We don't need to since we are built in the same
# cmake invocation as the it. That means we're being processed during the same configuration step as them, which
# means we'll have all their targets. Also, the targets that we reference in `target_link_libraries` commands don't
# even need to exist yet because CMake only validates the existence of targets at the end of the entire
# configuration process.

add_executable(${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToMeddyprojectDirPath)
target_compile_features(${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToMeddyprojectDirPath PUBLIC cxx_std_20)
target_sources(${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToMeddyprojectDirPath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/ProjectRootToMeddyprojectDirPath.cpp")
target_link_libraries(${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToMeddyprojectDirPath
  PRIVATE
    ${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Static
    Boost::filesystem
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.ProjectRootToMeddyprojectDirPath.UserVideos
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToMeddyprojectDirPath
    "/Users/Meddy Man/Videos"
    "/Users/Meddy Man/Videos/_meddyproject"
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.ProjectRootToMeddyprojectDirPath.AbsoluteRoot
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToMeddyprojectDirPath
    "/"
    "/_meddyproject"
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.ProjectRootToMeddyprojectDirPath.Empty
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToMeddyprojectDirPath
    ""
    "_meddyproject"
  )

add_executable(${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToProjectRootPath)
target_compile_features(${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToProjectRootPath PUBLIC cxx_std_20)
target_sources(${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToProjectRootPath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/MeddyprojectDirToProjectRootPath.cpp")
target_link_libraries(${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToProjectRootPath
  PRIVATE
    ${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Static
    Boost::filesystem
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.MeddyprojectDirToProjectRootPath.UserVideos
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToProjectRootPath
    "/Users/Meddy Man/Videos/_meddyproject"
    "/Users/Meddy Man/Videos"
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.MeddyprojectDirToProjectRootPath.AbsoluteRoot
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToProjectRootPath
    "/_meddyproject"
    "/"
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.MeddyprojectDirToProjectRootPath.Empty
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToProjectRootPath
    "_meddyproject"
    ""
  )

add_executable(${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToManifestFilePath)
target_compile_features(${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToManifestFilePath PUBLIC cxx_std_20)
target_sources(${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToManifestFilePath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/MeddyprojectDirToManifestFilePath.cpp")
target_link_libraries(${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToManifestFilePath
  PRIVATE
    ${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Static
    Boost::filesystem
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.MeddyprojectDirToManifestFilePath.UserVideos
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToManifestFilePath
    "/Users/Meddy Man/Videos/_meddyproject"
    "/Users/Meddy Man/Videos/_meddyproject/manifest.json"
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.MeddyprojectDirToManifestFilePath.AbsoluteRoot
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToManifestFilePath
    "/_meddyproject"
    "/_meddyproject/manifest.json"
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.MeddyprojectDirToManifestFilePath.Empty
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_MeddyprojectDirToManifestFilePath
    "_meddyproject"
    "_meddyproject/manifest.json"
  )

add_executable(${MY_BASE_PROJECT_NAME_FULL}_TryCreateNewProject)
target_compile_features(${MY_BASE_PROJECT_NAME_FULL}_TryCreateNewProject PUBLIC cxx_std_20)
target_sources(${MY_BASE_PROJECT_NAME_FULL}_TryCreateNewProject PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/TryCreateNewProject.cpp")
target_link_libraries(${MY_BASE_PROJECT_NAME_FULL}_TryCreateNewProject
  PRIVATE
    ${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Static
    Boost::filesystem
  )

add_test(
  NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.TryCreateNewProject.SuccessCase
  COMMAND ${CMAKE_COMMAND}
    "-DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}"
    "-DMY_TEST_EXECUTABLE_PATH=$<TARGET_FILE:${MY_BASE_PROJECT_NAME_FULL}_TryCreateNewProject>"
    "-DMY_TEST_WORKING_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Scripts/TryCreateNewProject_SuccessCase.cmake"
  )

add_executable(${MY_BASE_PROJECT_NAME_FULL}_GetOuterMeddyproject)
target_compile_features(${MY_BASE_PROJECT_NAME_FULL}_GetOuterMeddyproject PUBLIC cxx_std_20)
target_sources(${MY_BASE_PROJECT_NAME_FULL}_GetOuterMeddyproject PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/GetOuterMeddyproject.cpp")
target_link_libraries(${MY_BASE_PROJECT_NAME_FULL}_GetOuterMeddyproject
  PRIVATE
    ${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Static
    Boost::filesystem
  )

add_test(
  NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.GetOuterMeddyproject
  COMMAND ${CMAKE_COMMAND}
    "-DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}"
    "-DMY_TEST_EXECUTABLE_PATH=$<TARGET_FILE:${MY_BASE_PROJECT_NAME_FULL}_GetOuterMeddyproject>"
    "-DMY_TEST_WORKING_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Scripts/GetOuterMeddyproject.cmake"
  )
