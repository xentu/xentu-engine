# The Execution Process

Here is an outline on how I expect the execution process should go for a typical
game in the new version of Xentu:

## Base Initialization 

- The executable runs.
- VFS is setup so that relative relative files loaded.
- The chosen machine is created, and runs built-in scripts for the language.
- Machine looks for a game.py (or game.lua, game.js) and runs it.

## Script (python)

- User creates a XentuGame derived class.
- User instantiates a copy of the class, and passes it to xentu.run().
- Engine script communicates with c++ to create window/render surface.
- Game loop begins, calling update() and draw() respectively.
- Game ends when loop is broken or sys.exit() is called.

## Script (lua)

- User creates