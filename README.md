# Xentu Game Engine

A slim, fast and easy to learn framework for creating 2D games scripted in Lua. It's completely free to use, open-source (under the zlib license), written in C++, and very cross-platform friendly.

## Strengths

Xentu has a powerful, event driven, scripting design, which you won't find with other Lua powered game engines. This makes code written for Xentu very easy to understand, and helps the design of game code flow more naturally. Xentu also skips imposing an entity style system, so that you are not forced to use something that you may not ever fully utilise properly.

The initial goal of Xentu was to make an engine to power 2D RPG's and Visual Novel's on as many devices as possible. For the most part this is still the goal, however the engine is perfectly capable of powering many other types of 2D games too!

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
