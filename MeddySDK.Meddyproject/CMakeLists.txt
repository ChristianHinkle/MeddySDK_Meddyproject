
cmake_minimum_required(VERSION 3.8)

project("MeddySDK.Meddyproject" LANGUAGES CXX)

add_executable(MeddySDK.Meddyproject)

# Include CMake scripts
include(CMake/MeddySDK.Meddyproject.cmake)

# Configure MeddySDK.Meddyproject sources

target_sources(MeddySDK.Meddyproject PUBLIC ${MEDDYSDKMEDDYPROJECT_SOURCE_FILES_PUBLIC})

target_sources(MeddySDK.Meddyproject PRIVATE ${MEDDYSDKMEDDYPROJECT_SOURCE_FILES_PRIVATE})

# Configure MeddySDK.Meddyproject include directories

target_include_directories(MeddySDK.Meddyproject PUBLIC ${MEDDYSDKMEDDYPROJECT_SOURCE_PATHNAME_PUBLIC})

target_include_directories(MeddySDK.Meddyproject PRIVATE ${MEDDYSDKMEDDYPROJECT_SOURCE_PATHNAME})

# Enable C++20 if supported
if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MeddySDK.Meddyproject PROPERTY CXX_STANDARD 20)
endif()

# Enable Hot Reload for MSVC compilers if supported
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
