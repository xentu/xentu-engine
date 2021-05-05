# Xentu Game Engine

A slim, fast and easy to learn framework for creating 2D games scripted in Lua. It's completely free to use, open-source (under the zlib license), written in C++, and very cross-platform friendly.

## Strengths

- Just edit your Lua file, and play! No compile necessary.
- Assets load into special registers, making the engine run super fast.
- Coding with Lua is easier thanks the bundled custom event system.
- No need for SDL. Xentu provides an clever, highly configurable graphics system.
- Audio handled by the awesome cross-platform MiniAudio library.
- A custom built narrative system (with support for Twine & Arrow).
- Native support for tile map's from Tiled (tmx).
- Native support for sprite map's from Aseprite (json).
- Plays on more systems than any 2D engine, including all modern games consoles.

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

The engine is tested with GCC on windows via the CMake script. The subject is too complex to detail here, however you should ensure to downloads the platform specific versions of the dependencies above (.a/.dll/.lib). Placing them into a /lib/ folder is recommended, as the scripts in the cmake sub-folder check that directory when building.

In the future, our website & YouTube channel will include guides on how to build the project on all of the platforms we support.

## Contribution And Community

To get in touch, please use the Gitter board https://gitter.im/xentu-devs/community


## Contributors

This project is maintained by: 

* [@kodaloid](https://github.com/kodaloid)
