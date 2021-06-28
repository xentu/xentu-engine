# Xentu Game Engine

A slim, fast and easy to learn framework for creating 2D games scripted in Lua.
It's completely free to use, open-source (under the zlib license), written in
C++, and very cross-platform friendly.

## Dependencies

- GLEW
- GLFW3
- Lua 5.3.5
- Luna
- Mini Audio
- nlohmann/json
- STB Image
- TinyToml

## Building

The engine is tested with GCC (vscode-linux), GCC/MinGW32 (vscode-win), and
MSVCC (vs-win) using CMake. I plan to make this more straight forward soon.
However for now here are some platform specific notes to get you started.

Note for the most part testing has taken place inside VSCode with the CMake and
C++ extensions enabled.

### Windows Notes

If you clone the repo with intent on building from source, and want to build
as-is without changes, remember to do the following:
- Create a folder called lib, lib/GLEW, lib/GLFW3, lib/lua535
- Download each respective library binary, and place in the correct folders.
- For MSVC, Lua is static linked (lua53.lib), GLEW and GLFW3 shared (glew32.lib, glew32.dll, libglfw3dll.a,  glfw3.dll)
- For GCC, Lua is static linked (liblua53.a), GLEW and GLFW3 shared (glew32.lib, glew32.dll, libglfw3.a, glfw3.dll)

### Linux Notes

- Compiling on Linux has been tested with GCC 7.3, 8.4 and 9.3 and works.
- You might get an error saying GLSL 3.3 is not available using the test shaders.
  While not the best solution, you can try "export MESA_GL_VERSION_OVERRIDE=3.3"
  before running to make your distro use the compatibility profile.
- To compile on Linux Mint v20, I needed to install (with apt): libgl1-mesa-dev,
  libglu1-mesa-dev, liblua5.3-0, libglew2.1, libglfw3, build-essential

## Documentation

- Documentation is compiled using Sphinx, and is currently hosted at the following URL: https://docs.xentu.net

## Contribution And Community

To get in touch, please use the Gitter board https://gitter.im/xentu-devs/community


## Contributors

This project is maintained by: 

* [@kodaloid](https://github.com/kodaloid)
