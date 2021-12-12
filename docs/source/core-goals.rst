==========
Core Goals
==========

The way you create something is important, especially when you are creating it
to share with others. The following core goals explain the direction of this
project. And should act as rules & guidance on how the project should move forward.


1. Maintain A Responsible API
-----------------------------

Xentu is an API first game engine. Which means the syntax/layout of how you
program a game using Xentu is hugely important. Weather it be a game built for a
web browser, a games console, or a mobile phone, the Lua code you write must work
without modification. For this reason, all maintained official ports of the base
engine must adhere to the API specification.


2. Remain Free
--------------
No exceptions. The main source, and all libraries we link too must be ZLib, MIT,
un-license, BSD 2-Clause or equivalent. If a 3rd party library changes to a license
that adds restrictions, then we must at earliest convenience replace that library
with another that fits this principle, and push for a release ideally before that
new license comes into effect.


3. Be Accessible
----------------
The engine must always be easy to get hold of, easy to learn and use. The goal is
to have the following:

- SDK/IDE available through package managers (winget, chocolatey, apt, pacman,
  pip, yum, homebrew). installers available for Windows, Linux (Flatpak), MacOS
  and HaikuOS (cli only).
- A well written and maintained documentation implemented using Sphinx,
  supplemented with videos hosted on YouTube to make it easier for first timers
  to get set-up.
- Provide up-to date examples of how to build a number of basic games using the
  engine, with video tutorials where possible.
- Prepare and present talks at game dev events and game jams to introduce people
  and show them how they can use the engine.


4. Support Minimum Spec
-----------------------
A milestone goal of v1.0.0 is to have a minimal acceptable performance for a
specific base level of hardware (lets say the Raspberry Pi Zero W, or a N3DS for Consoles).

Aiming for the lowest common denominator drops the barrier to entry. It allows
games developers who wish to maintain their games for years to come, the ability
to know they'll be able to use future updates of the engine without breaking
their games, and it has the side effect of forcing a level of quality control.

As a rule, the engine should be able to run within these conditions:

- Resolution: 160x120
- RAM: 64MB
- VRAM: 3MB
- Disk Space: 16MB

Considering the above, community pull requests for things that do not meet the
above requirement should be immediately rejected (say request to add something
that pushes the min vram above 3MB).
