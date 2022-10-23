<p align="center"><img width="100" src="assets/images/logo.png" alt="Xentu logo" /></p>
<h1 align="center">Xentu Game Engine</h1>

A slim, fast and easy to learn framework for creating 2D games scripted in Lua,
Python or JavaScript. It's completely free to use, open-source (under the zlib license), written in C/C++, and super cross-platform friendly.

Xentu is designed to be compiled and ran on a vast number of different systems.
It runs great on Windows, Linux/BSD, MacOS, ChromeOS, HaikuOS, surprisingly well
on ARM based computers like the Raspberry PI. Games consoles and mobile are on the
horizon too, along with Emscripten which will allow you to run your games in a
web browser.

If you want the most time saving and hassle free way to make your next computer
game work in as many places as possible (not just those with a browser available), Xentu has your back!

<br />


## Features

- Uses SDL2 backend, with OpenGL 3.3+ (and DirectX mode coming soon).
- Write your game code in Lua, JavaScript, or Python (disabled temporarily).
- Load textures, sprites, shaders, fonts, and more using the asset manager.
- Store your assets in archives, and access them via the built in VFS (virtual file system).
- Render smartly with a fully featured and efficient batch manager.

<br />


## Getting Started

If this is your first time using Xentu, you should try the also free [Xentu Creator](https://xentu.net/creator) the official cross-platform editor IDE for Xentu. It provides a rich code editor, debugging, project management, and other cool things like code templates to get you started.

If you'd prefer to go it alone, the binaries can be downloaded from the Xentu
website using [this link](https://xentu.net/binaries). And in the next section
you'll see how to build the engine yourself.

<br />


## Building

The project requires meson to build, which also requires Python3+. Once you have
both installed, run the setup.sh script to setup dependencies. If you are not on Windows, you may need to make the sh file executable first with `"chmod +x ./setup.sh"`. Once ready, calling:

```meson compile -C build```

or

```ninja compile -C build```   (on some older systems)


Will compile the engine into `/build/xentu.exe` on windows, or `/build/xentu` on
MacOS/Linux respectively. Also if you have VSCode, a tasks.json has been setup
so you can build using the build command, and a launch.json has been provided to
help aid in launching the engine in debug mode.

<br />

## Contribution And Community

To get in touch, please join the discord at https://discord.com/invite/enC7FGvV

<br />

## Contributors

This project is maintained by: 

* [@kodaloid](https://github.com/kodaloid)

<br />