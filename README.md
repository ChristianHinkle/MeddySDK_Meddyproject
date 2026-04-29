# MeddySDK_Meddyproject

The Meddy project system. A library with tools to interact with "meddyprojects".

For a superproject with all components MeddySDK has to offer, see [MeddySDK](https://github.com/ChristianHinkle/MeddySDK).
- This provides the individual MeddySDK libraries, tied together with CMake.

## Dependencies

MeddySDK_Meddyproject
- [Boost.Filesystem](https://github.com/boostorg/filesystem)
- [RapidJSON](https://github.com/Tencent/rapidjson)
- [CppUtils_Core](https://github.com/ChristianHinkle/CppUtils_Core)
- [CppUtils_Misc](https://github.com/ChristianHinkle/CppUtils_Misc)
- [CppUtils_StdReimpl](https://github.com/ChristianHinkle/CppUtils_StdReimpl)

## Project Structure 📂

Does it install: Yes, it's installed as the `MeddySDK_Meddyproject` package and export.

### Include/

Provides the target: `MeddySDK::Meddyproject::Include` (interface library).

Does it install: Yes, it's installed as the `Include` component.

What it is: Holds all the public header files of our source code. This can be used as a header-only library.

### Source/

Provides the target: `MeddySDK::Meddyproject::Source` (interface library).

Does it install: Yes, it's installed as the `Source` component.

What it is: Holds all the private source files to compile.

### Static/

Provides the target: `MeddySDK::Meddyproject::Static` (static library).

Does it install: Yes, it's installed as the `Static` component.

### Shared/

Provides the target: `MeddySDK::Meddyproject::Shared` (shared library).

Does it install: Yes, it's installed as the `Shared` component.

### Module/

Provides the target: `MeddySDK::Meddyproject::Module` (module library).

Does it install: Yes, it's installed as the `Module` component.

### Object/

Provides the target: `MeddySDK::Meddyproject::Object` (object library).

Does it install: Yes, it's installed as the `Object` component.

### Tests/

Does it install: No, but we should support this so that dependent projects can use our test code to help with writing their own tests.

What it is: Provides automated tests. See "Test Instructions" for how to use.

## Build System ⌨

Built with CMake - cross-platform, standardized, and IDE-friendly.

We provide CMake presets, which handle feeding arguments to CMake for you.

### IDE Support

Most IDEs provide built-in CMake integration.

#### VS Code

Has the "CMake Tools" and "C/C++" extensions, both developed by Microsoft.

#### Visual Studio

Has very nice integration, but they seem behind when it comes to supporting the latest CMake features. I've had experiences where I have to switch to VS Code because of this.

## Build Instructions 🔨

This project is fully isolated from its dependencies, and therefore must be tied together by a superproject. See [MeddySuperproject](https://github.com/ChristianHinkle/MeddySuperproject) for a complete build setup, as well as instructions for how to build, package, and test.
