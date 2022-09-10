# The Execution Process

Here is an outline on how I expect the execution process should go for a typical
game in the new version of Xentu:

## Base Initialization 

- The executable runs.
- VFS is setup so that relative relative files loaded.
- The chosen machine is created, and runs built-in scripts for the language.
- Machine looks for a game.py (or game.lua, game.js) and runs it.

## Script Initialization

- Create a window and renderer pair (may allow multiple pairs in future).
- Hook into global events such as update and draw.
- Call game.run() to start the game loop.

## Game Loop

The game loop is controlled by the initialized machine. It's a closed loop that
runs as fast as possible, and triggers events based on the timings specified in
the config.