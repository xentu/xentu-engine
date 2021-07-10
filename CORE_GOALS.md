## Maintain A Responsible API
Xentu is an API first game engine. Which means the syntax/layout of how you
program a game using Xentu is hugely important. Weather it be a game built for a
web browser, a games console, or a mobile phone, the Lua code you write must work
without modification. For this reason, all maintained official ports of the base
engine must adhere to the API specification.

## Remain Free
No exceptions. The main source, and all libraries we link too must be ZLib, MIT,
un-license, BSD 2-Clause or equivalent. If a 3rd party library changes to a license
that adds restrictions, then we must at earliest convenience replace that library
with another that fits this principle, and push for a release ideally before that
new license comes into effect.

## Be Accessible
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

## Support Minimum Spec
Adding a requested feature is not an excuse to exclude older hardware. A milestone
goal of v1.0.0 is to have a benchmark project that can run within the following
specification:

- Min Framerate: 30fps
- Resolution: 160x120
- RAM: 64MB
- VRAM: 3MB
- Disk Space: 16MB

This is not a requirement for people using the engine. Rather a fundamental aim
of the project. Enforcing this minimum spec ensures that the barrier to entry
remains fair, and it also allows for further retro ports in the future (like 
older or retro games consoles, web browsers via emscripten etc...).

For this reason pull requests for things that do not need to be embedded into
the engine should be immediately rejected. And feature requests must show a
legitimate reason for being considered. It's important to understand that many
engine projects welcome such requests, but such an open door policy conflicts
with the principle of maintaining support for a minimum spec.

## Encourage & Facilitate Video Game Preservation
We want games made in 2021 using Xentu to still be playable in 2121. The only way
we can stand any chance of aiding that, is to work hard enact all of the core
principles of this project.

To go a step further, we will run a voluntary open database of projects that
wish to be counted as preserve-able. The database will be hosted on the xentu.net
website, with the hope that we can encourage more creators to think about the
future and preservation when creating their art.