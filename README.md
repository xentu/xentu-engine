# Xentu Game Engine

A slim, fast and easy to learn framework for creating 2D games scripted in Lua.
It's completely free to use, open-source (under the zlib license), links to other
open-source libraries, written in C/C++, and very cross-platform friendly.

<br />

## Engine Features

- Works on OpenGL 4.1+ (and ES 3.0+).
- Game code is written in Lua, no need to recompile every time.
- Structure using very fast emitter/subscriber events.
- Organise with the scene system (switch instantly between game parts).
- Load in textures, sprites, custom shaders, fonts, data etc... using a clever asset manager.
- Renderer smartly with a system that draws in pre-organised batches for performance.
- Move, rotate or scale graphics using the simple built-in transform system.
- Includes tools for creating, packaging and sharing your games.
- Compact & portable, with the main exe compiling to 2mb in release mode.

<br />

## Main Links

- Main Website: [https://xentu.net](https://xentu.net)
- Documentation: [https://docs.xentu.net](https://docs.xentu.net)

## Table Of Contents

- [Background](#background)
- [Dependencies](#dependencies)
- [Building](#building)
- [Documentation](#documentation)
- [Contribution And Community](#contribution-and-community)
- [Contributors](#contributors)

<br />

## Background

The engine works, but it's still far from a v1.0. Development (with prototyping)
has taken the best part of 3 years so far, so don't let the commit log fool you.

Original prototype development started early 2018 on top of the MonoGame framework
in C# and the NLua bindings. The project lasted till November 2019, where it was
decided to move to C++ due to wanting more control and performance.

The concern was that the Lua cross-talk (while optimal thanks to NLua) was still
slow, and hurt performance when tied to the game loop. It was theorised that a
straight implementation without overhead would alleviate the problem, which it
did.

The C++ concept prototype was completed in May 2020 showing the implementation was
much stronger performance wise. And so development of the project began in earnest
on the 3rd of May 2020.

<br />

## Going Forward (as of December 15th)

The Windows installer now works, and the documentation has been completed to a
point of basic competence. A lot of work has been done getting the infrastructure
for the website up and running, and problems where the SDK environment path was 
not being set has been rectified (if a bit patchy still).

Here is a list of todos going forward:

- Turn the things in this list into GitHub issues.
- Finish the website, and set the repo as public.
- The game singleton is called to retrieve the lua_State sometimes when it shouldn't.
- Not enough validation is carried out when calling native methods via Lua.
- Reduce the number of times the engine crashes out due to a basic error, things like loading assets.
- Add a Math library for calculating transforms, this should simplify things.
- It's currently not possible to send custom information to shaders.
- Fix the SDK install routine where it sometimes adds too much to the path variable.
- Create an icon for the SDK, and finish the basic help page.
- Get a code signing certificate to add trust for the SDK download.

<br />

## Dependencies

This project relies on the following libraries:

- GLEW
- GLFW3
- Lua 5.3.5
- Luna
- Mini Audio
- nlohmann/json
- STB Image
- TinyToml
- TmxLite

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
