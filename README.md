<p align="center"><img width="100" src="assets/xentu-logo.png" alt="Xentu logo" /></p>
<h1 align="center">Xentu Game Engine</h1>

A slim, fast and easy to learn framework for creating 2D computer games scripted
in a variety of scripting languages like Lua, JavaScript and more. It's
completely free to use & publish with. Open-source (under the zlib license),
written in C/C++, and super cross-platform friendly.

With this latest version we are introducing web-browser support via an Emscripten
build chain. This means not only can you run games natively on all sorts of hardware,
but it'll also work on any device that has a browser. This makes it incredibly easy
to get your games running on mobile & tablet within minutes.

Xentu still works fantastic on native hardware, though now have more options than
ever to share your creations!

<br />


## Features

- Draw graphics with a fully featured and fast batch renderer.
- Load textures, shaders, fonts, data and more using a smart asset manager.
- Support for sprite maps & tile maps (tmx support).
- Input support for keyboards, mice and game pads.
- Sound and music playback, with 8 mixer tracks.
- Divide a game up into logical scenes with the scene manager.
- Run easy to program game sequences using the built-in story engine.

<br />


## About this branch

The last edition of Xentu was an official alpha release [v0.0.6 (Alpha)](https://github.com/xentu/xentu-engine/releases/tag/v0.0.6-alpha). That version
on its own has been good enough to power a few games of mine and others, but it 
felt incomplete. Plus it was hard to maintain due to certain external dependencies
being unreliable. At the beginning of 2024 I decided to put pride aside and finally
learn how to port the engine to web. Due to work and other commitments, it's taken
me a while to get here, but alas after 1.5 years here we are.

This version of the engine has all the same features (don't believe me? compare the /src
folder!) the biggest changes are these;

- JS and Python are gone for now leaving Lua (but they will be back soon).
- Emscripten has a different file system, so we are back to per-platform asset loading for now.
- Meson has been replaced with CMake (Yea I finally learnt CMake, which makes dual builds much easier).
- A new version of Xentu Creator is also on the way (Thank the abysmal MS Monaco editor for forcing my hand on that one by being so unmaintainable it borked recent builds).

<br />

## Getting Started

In a few weeks time this branch will become the master enabling a 0.1 alpha to be produced. But for now
if you want to try it out, clone this branch and try build it with Emscripten (if you use CLion the build target DebugWasm)
should be available. Building should generate the .wasm and .js files in the bin folder, allowing you to give the
engine a go. I'll also shortly be posting a web playground that you can use to try Xentu via the website.

<br />

## What about the old version?

Nothing will happen to it, in-fact the new XentuCreator app will still be able to target it if you wish
to use the older version for specific targets.

<br />

## Documentation

Full documentation for this project can be found at [https://docs.xentu.net](https://docs.xentu.net), and the GitHub repo for it can be found [here](https://github.com/xentu/xentu-docs).

<br />

## Contribution And Community

To get in touch, please join the discord at https://discord.gg/Zpb5dYrhRa

<br />

## Contributors

This project is maintained by:

* [@kodaloid](https://github.com/kodaloid)

<br />