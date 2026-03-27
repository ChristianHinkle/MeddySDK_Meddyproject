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

add_executable(${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToDotMeddyprojectPath)
target_compile_features(${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToDotMeddyprojectPath PUBLIC cxx_std_20)
target_sources(${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToDotMeddyprojectPath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/ProjectRootToDotMeddyprojectPath.cpp")
target_link_libraries(${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToDotMeddyprojectPath
  PRIVATE
    ${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Static
    Boost::filesystem
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.ProjectRootToDotMeddyprojectPath.UserVideos
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToDotMeddyprojectPath
    "/Users/Meddy Man/Videos"
    "/Users/Meddy Man/Videos/.meddyproject"
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.ProjectRootToDotMeddyprojectPath.AbsoluteRoot
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToDotMeddyprojectPath
    "/"
    "/.meddyproject"
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.ProjectRootToDotMeddyprojectPath.Empty
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_ProjectRootToDotMeddyprojectPath
    ""
    ".meddyproject"
  )

add_executable(${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToProjectRootPath)
target_compile_features(${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToProjectRootPath PUBLIC cxx_std_20)
target_sources(${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToProjectRootPath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/DotMeddyprojectToProjectRootPath.cpp")
target_link_libraries(${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToProjectRootPath
  PRIVATE
    ${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Static
    Boost::filesystem
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.DotMeddyprojectToProjectRootPath.UserVideos
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToProjectRootPath
    "/Users/Meddy Man/Videos/.meddyproject"
    "/Users/Meddy Man/Videos"
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.DotMeddyprojectToProjectRootPath.AbsoluteRoot
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToProjectRootPath
    "/.meddyproject"
    "/"
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.DotMeddyprojectToProjectRootPath.Empty
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToProjectRootPath
    ".meddyproject"
    ""
  )

add_executable(${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToManifestFilePath)
target_compile_features(${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToManifestFilePath PUBLIC cxx_std_20)
target_sources(${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToManifestFilePath PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/DotMeddyprojectToManifestFilePath.cpp")
target_link_libraries(${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToManifestFilePath
  PRIVATE
    ${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Static
    Boost::filesystem
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.DotMeddyprojectToManifestFilePath.UserVideos
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToManifestFilePath
    "/Users/Meddy Man/Videos/.meddyproject"
    "/Users/Meddy Man/Videos/.meddyproject/Manifest.json"
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.DotMeddyprojectToManifestFilePath.AbsoluteRoot
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToManifestFilePath
    "/.meddyproject"
    "/.meddyproject/Manifest.json"
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.DotMeddyprojectToManifestFilePath.Empty
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_DotMeddyprojectToManifestFilePath
    ".meddyproject"
    ".meddyproject/Manifest.json"
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
