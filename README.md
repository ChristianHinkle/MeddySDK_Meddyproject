# MeddySDK_Meddyproject

For a superproject with all components Meddy has to offer, see https://github.com/ChristianHinko/MeddySDK.
- This provides the individual MeddySDK libraries, tied together with CMake.

## Dependencies

MeddySDK_Meddyproject has zero dependencies so far.

## Build System

Everything here is built off of CMake, which is cross-platform, and gets great support from IDEs.

Our "CMakePresets.json" file takes care of feeding the right arguments to CMake, and provides presets for building, packaging, etc.

### IDE Support

Most IDEs provide built-in CMake features.

VS Code has the "CMake Tools" and "C/C++" extensions, developed by Microsoft.

Visual Studio has very nice integration, but they seem behind when it comes to supporting the latest CMake features. I've had experiences where I have to switch to using VS Code because of this.

## Build Instructions

This project is fully isolated from its dependencies, and therefore must be tied together by a superproject. See https://github.com/ChristianHinko/MeddySDKStandalone for a complete build setup.

### 1. Invoke CMake on the Project (the Configure Step)

Command line: `cmake --preset="windows-x64-debug"`.

IDE: Choose the "windows-x64-debug" configure preset, and "configure" the CMake project.

### 2. Invoke a Build Command

Command line: `cmake --build --preset="windows-x64-debug"`.

IDE: Choose the "windows-x64-debug" build preset, and "build" it.
