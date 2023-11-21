<p align="center"><img width="100" src="demos/main/images/logo.png" alt="Xentu logo" /></p>
<h1 align="center">Xentu Game Engine</h1>

A slim, fast and easy to learn framework for creating 2D computer games scripted 
in a variety of scripting languages, including JavaScript, Lua and more. It's 
completely free to use & publish with. Open-source (under the zlib license), 
written in C/C++, and super cross-platform friendly.

Web browsers have the power to enable people to build interactive experiences 
using popular scripting languages like JavaScript, Lua, Ruby & Python. But they 
come at the cost of having a big browser attached with all it's system overheads. 

Xentu aims to strip away this big clunky browser, and make games written in those popular 
languages available to run pretty much anywhere natively.

<br />


## Features

- Draw graphics with a fully featured and fast batch renderer.
- Load textures, shaders, fonts, data and more using a smart asset manager.
- Access assets from archives via the built in VFS (virtual file system).
- Support for sprite maps & tile maps.
- Input support for keyboards, mice and game pads.
- Sound and music playback, with 8 mixer tracks.
- Divide a game up into logical scenes with the scene manager.
- Run easy to program game sequences using the built in story engine.

<br />


## Getting Started

If this is your first time using Xentu, you should try [Xentu Creator](https://github.com/xentu/xentu-creator) the official cross-platform editor IDE for Xentu. It provides a rich code editor, debugging, project management, and other cool things like code templates to get you started.

If you'd prefer to go it alone, the binaries can be downloaded from the releases
page [here](https://github.com/xentu/xentu-engine/releases). And in the next section
you'll see how to build the engine yourself.

<br />


## Documentation

Full documentation for this project can be found at [https://docs.xentu.net](https://docs.xentu.net), and the GitHub repo for it can be found [here](https://github.com/xentu/xentu-docs).

<br />


## Building

The project requires meson to build, which also requires Python 310 or higher. With
both installed, run the setup.sh script to setup dependencies. If you are not on
Windows, make sure the sh file is marked as executable with `"chmod +x ./setup.sh"`. 
Once ready, call:

```meson compile -C build```

or

```ninja -C build```

to compile the engine binaries into the `build` folder named `xentu` and `xentu_debug`.
Also if you have VSCode, a tasks.json has been setup so you can build using the 
build command, and a launch.json has been provided to help aid in launching the 
engine in debug mode.

If you find the binaries that are produced are too large, you likely need to
switch to a different buildtype in the meson.build file. The common options are
'debug', 'release' and 'minsize'.

### Machines

Each language supported by the engine is managed by a "machine". In each of the
machine folders, you'll find scripts that are loaded when the engine starts. If
you change those files, make sure to run the `setup-machines.py` script, which
will convert the scripts into C++ header files, readable by meson.

### Building On Linux

Xentu requires Lua 5.4, Python 3.10 and GLEW to compile successfully. Currently
Linux Mint ships with everything, although you may need to apt install 
`freeglut3`, `freeglut3-dev` and `libglew-dev`.

## Python Support

Python does not get statically linked during compile time, as it adds a lot of additional bulk. Instead two additional binaries are compiled with names ending in `_py`. If you ship a game using Python on Windows, you should also ship `python310.dll` with your game, just in-case your players do not have it.

<br />

## Contribution And Community

To get in touch, please join the discord at https://discord.gg/Zpb5dYrhRa

<br />

## Contributors

This project is maintained by: 

* [@kodaloid](https://github.com/kodaloid)

<br />
