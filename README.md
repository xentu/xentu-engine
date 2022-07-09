<p align="center"><img width="100" src="assets/logo.png" alt="Xentu logo" /></p>
<h1 align="center">Xentu Game Engine (v0.0.2 rebuild)</h1>


A slim, fast and easy to learn framework for creating 2D games scripted in Lua,
Python, Golang or JavaScript (via Ducktape). It's completely free to use, open-source
(under the zlib license), links to other open-source libraries, written in C/C++,
and very cross-platform friendly.

<br />

## Engine Features

- Works on OpenGL 3.3+ (and ES 3.0+) or SDL.
- Game code is written in Lua (or Python/Golang), no need to recompile every time.
- Structure using very fast emitter/subscriber events.
- Organise with the scene system (switch instantly between game parts).
- Load in textures, sprites, custom shaders, fonts, data etc... using a clever asset manager.
- Renderer smartly with a system that draws in organised batches for performance.
- Move, rotate or scale graphics using the simple built-in transform system.
- Includes tools for creating, packaging and sharing your games.
- Compact & portable, with the main exe compiling to 2mb in release mode.

<br />

## This Is A Development Branch

If you want a working engine, see the [master branch](https://github.com/xentu/xentu-engine/tree/master).

For the next Alpha (v0.0.2), I am rebuilding Xentu so that it can use more modern
tech, add support for other scripting languages (Python, Golang, JavaScript), and
include support for other projects I have like the Sutori dialog engine. This track
will also add support for emscripten so that we finally get browser support.

Project brief:
- Use meson/ninja instead of CMake. [done]
- Fix, port & update vfspp so that xentu has a real virtual file system. [done]
- Make the engine more abstraction orientated allowing for parts to be swapped.
- Add core functionality to support embedded languages.
- Add SDL compatibility.

## Building

The project now requires meson to build which also requires Python3+. Once you have
both installed, run the setup.sh script to setup dependencies. Once ready, calling

```
meson compile -C build
```

Will compile the engine into `/build/xentu.exe` on windows, or `/build/xentu` on
MacOS/Linux respectively. Also if you have vscode, a tasks.json has been setup
so you can build using the build command, and a launch.json has been provided to
help aid in launching the engine in debug mode.

<br />

## Contribution And Community

To get in touch, please join me on discord at https://discord.com/invite/enC7FGvV

<br />

## Contributors

This project is maintained by: 

* [@kodaloid](https://github.com/kodaloid)

<br />

## Background

The initial alpha release of the Xentu game engine is a major milestone. It does
everything I set out to do, allows you to write a game in Lua, and play it on many
platforms via a C++/OpenGL engine. But that completion has not arrived without
flaws.

Whilst building the alpha, I also learnt C++/CMake. This inevitably means I made
mistakes that are now embedded deep in the design of the code. A good example of
this is how things are loaded into the game. Currently it works like this:

- Core scripts/shaders etc.. are embedded into the executable via CMake. 
- An asset manager on the C++ side can load in files from the filesystem.
- The Lua interpreter can speak to the filesystem to require other lua files.

That all sounds fine until you realise from a programmers perspective it's highly
unmaintainable. As a result I was never able to provide a way to wrap a games
assets into a bundle like other games do, and I also had to write an alternative
include lua function to get around Lua's strange include system.

### CMake Issues

Another example is how I was forced to setup CMake. If you follow my instructions
from the README.md to the letter, it will build. But that's only because I ask
that you use a specific compiler known to work.

There are people who will disagree with me, but in my experience CMake is a hot
mess. It's great when it works, but the poor documentation is crippling when you
are a sole developer trying to build something like Xentu. In some situations you
can end up spending weeks trying to understand a single command, only to find out
in some message board from 15 years ago that it was abandoned and replaced with
something new. 

Going forward CMake needs replacing, so I'm moving over to a more modern build
system called meson, which crucially gives the ability to compile using different
compilers, and provides a far smoother build dynamic.

### Language Concerns

Lua in my opinion is one of the most underrated programming languages, and for a
long time I've pushed it as the only choice for Xentu. It's speed, learnability 
and elegance is such an obvious choice for enabling people to make games, but
maybe my obsession is clouding my judgement.

If you run code for a language natively, package managers can bridge gaps in
functionality, but when it comes to embedded languages, it's quite rare to find
that those packages can accompany the host application. Compiled languages like
C# and C++ have an advantage as you can just bundle the package .dll/.so files,
but embedded languages usually have no equivalent, unless the host application
requires the user to also install the needed package manager.

Due to this, it's actually quite important for the language being embedded to
provide everything that you'll need ot of the box. For the Xentu alpha, Lua did
provide most of what I needed, but there were some noticeable gaps that caused
a few problems. For example when I had to load XML in C++ instead of Lua when 
loading Tiled TMX files.

Recently whilst working on another project called Sutori, a dialog engine for the
likes of quizzes and visual novels (which is primarily a javascript endeavour) I
began writing ports to other languages. Ports for .NET, and Python were as simple
as you can imagine, however when I went to write a port for Lua (I obviously wanted
it to work with Xentu) I realised that I'd have to do the whole Tiled TMX thing
for Sutori, so what took maybe an hour or two in .NET and Python was going to take
a few days in Lua!

So here's the deal, Lua is still going to be a major language supported by the
engine. But for this track it will take a slight back seat to allow for me to
implement a core set of libraries in c++ that allows Lua (and other interpreters)
to have the same out of the box functionality that Python has.

Here's a list of libraries that should be getting added to the oobf list:

- Serializer/Deserializer's for: XML, JSON, TOML, CSV
- Encoder/Decoders for base16, base32, base64
- Compression library for: zip, bzip2, gzip, lzma, zlib
- Hashing library like [hashlib](https://docs.python.org/3/library/hashlib.html)
- Logging library.
- RegEx library.

If you have any suggestions for further libraries, please let me know.

## Whats Next

All the pieces needed to get this done are easily available, and the vfs is already
fully working. If everything goes to plan, this alpha will be complete by the end
of september 2022, and may end up as the beta if everything goes well.