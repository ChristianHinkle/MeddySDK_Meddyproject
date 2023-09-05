
cmake_minimum_required(VERSION 3.8)

project("MeddySDK.Meddyproject" LANGUAGES CXX)

add_executable(MeddySDK.Meddyproject)

target_sources(MeddySDK.Meddyproject
  PRIVATE
    "Source/Private/MeddySDK.Meddyproject.cpp"
  )

target_include_directories(MeddySDK.Meddyproject
  PUBLIC
    "Source/Public"
  PRIVATE
    "Source"
  )

# Enable C++20 if supported
if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MeddySDK.Meddyproject PROPERTY CXX_STANDARD 20)
endif()

# Enable Hot Reload for MSVC compilers if supported
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
