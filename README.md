# Xentu Game Engine

A slim, fast and easy to learn framework for creating 2D games scripted in Lua.
It's completely free to use, open-source (under the zlib license), links to other
open-source libraries, written in C++, and very cross-platform friendly.

For more information on what the core goals of the engine are please check out:
[CORE_GOALS.md](CORE_GOALS.md).

And for even more information, why not visit the official website!:
https://xentu.net.

<br />

## Dependencies

- GLEW
- GLFW3
- Lua 5.3.5
- Luna
- Mini Audio
- nlohmann/json
- STB Image
- TinyToml

<br />

## Building

The engine is written in C++ 14 standard, and is set-up to be built with CMake.
Generally if an OS is supported by the dependencies, can compile using CMake,
and has support for OpenGL 4.1, then you can built it.

Here is a bare metal checklist for how to build the engine on Windows within VS Code:
1. Install GCC 8 or higher (mingw in i686 is a good shot: http://mingw-w64.org/).
2. Verify your console can find GCC by using `gcc --version`, if not fix first.
3. Install CMake (https://cmake.org/download/)
4. Install the CMake Tools extension for VS Code.
3. Clone this repo, open folder with VS Code, allow CMake Tools to auto-config.
4. Either use the keyboard shortcut F7 or use the CMake panel in VSCode to build.
5. Both executable files will appear in the generated out folder.

If you prefer using Visual Studio, the CMake project is also compatible, just 
make sure you have CMake installed, there is no need for GCC using this method.

### Windows Notes

If you clone the repo with intent on building from source, and want to build
as-is without changes, remember to do the following:
- Create a folder called lib, lib/GLEW, lib/GLFW3, lib/lua535
- Download each respective library binary, and place in the correct folders.
- MSVC: Lua is static (lua53), GLEW/GLFW3 shared (glew32, glfw3)
- GCC: Lua is static (liblua53), GLEW/GLFW3 shared (glew32, libglfw3, glfw3)

### Linux Notes

- Compiling on Linux has been tested with GCC 7.3, 8.4 and 9.3 and works.
- To compile on Linux Mint v20, I needed to install (with apt): libgl1-mesa-dev,
  libglu1-mesa-dev, liblua5.3-0, libglew2.1, libglfw3, build-essential

### MacOS Notes

Compiling is very similar to Linux (above), however you should make sure the 
device is OpenGL 4.1 compatible. I have tested on a MacMini from 2012, and it 
barely meets the spec.

<br />

## Documentation

Documentation is compiled using Sphinx, and is currently hosted at the following
URL: https://docs.xentu.net

<br />

## Known Issues

The engine has a resource system where resources are built during CMake config.
The system uses a assembly trick with switches that should work on most operating
systems. However only Windows, Linux 64bit and MacOS Catalina have been tested,
so your milage may vary. The mechanism can be explored in `cmake/FindEmbed.cmake`

<br />

## Contribution And Community

To get in touch, please use the Gitter board https://gitter.im/xentu-devs/community

<br />

## Contributors

This project is maintained by: 

* [@kodaloid](https://github.com/kodaloid)
