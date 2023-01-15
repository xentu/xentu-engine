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
- Load textures, shaders, fonts, and more using a smart asset manager.
- Access assets from archives via the built in VFS (virtual file system).
- Support for sprite maps & tile maps.
- Input support for keyboards, mice and game pads.
- Sound and music playback, with 8 mixer tracks.

<br />


## Getting Started

If this is your first time using Xentu, you should try [Xentu Creator](https://xentu.net/creator) the official cross-platform editor IDE for Xentu. It provides a rich code editor, debugging, project management, and other cool things like code templates to get you started.

If you'd prefer to go it alone, the binaries can be downloaded from the Xentu
website using [this link](https://xentu.net/binaries). And in the next section
you'll see how to build the engine yourself.

<br />


## Documentation

Full documentation for this project can be found at [https://docs.xentu.net](https://docs.xentu.net), and the GitHub repo for it can be found [here](https://github.com/xentu/xentu-docs).

<br />


## Building

The project requires meson to build, which also requires Python3+. Once you have
both installed, run the setup.sh script to setup dependencies. If you are not on Windows, you may need to make the sh file executable first with `"chmod +x ./setup.sh"`. Once ready, calling:

```meson compile -C build```

or

```ninja -C build```

Will compile the engine into `/build/xentu.exe` on windows, or `/build/xentu` on
MacOS/Linux respectively, and a `xentu_debug.exe` that is designed for debugging.
Also if you have VSCode, a tasks.json has been setup so you can build using the 
build command, and a launch.json has been provided to help aid in launching the 
engine in debug mode.

If you find the binaries that are produced are too large, you likely need to
switch to a different buildtype in the meson.build file. The common options are
'debug', 'release' and 'minsize'.

## Building Notes (Linux)

Xentu requires Lua 5.4, Python 3.10 and GLEW to compile successfully. Currently
Linux Mint ships with everything, although you may need to apt install 
`freeglut3`, `freeglut3-dev` and `libglew-dev`.

## Python Support

Python is a unique case in that it does not get statically linked during compile
time. Due to this two additional binaries are compiled with a `_py` postfix. If
you ship a game using Python on Windows, remember to also ship python310.dll with
your game just in-case your players do not have it.

<br />

## Contribution And Community

To get in touch, please join the discord at https://discord.com/invite/enC7FGvV

<br />

## Contributors

This project is maintained by: 

* [@kodaloid](https://github.com/kodaloid)

<br />