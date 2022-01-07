===============
Getting Started
===============

Whenever you start work on a project in Xentu, you should create a folder to
host the files within.

Every game you create needs a ``game.lua`` file that contains game code,
and an optional ``config.toml`` file to configure how the engine should behave
as it loads. You can also include a ``game.ico`` if you wish to set an app
icon for your game.

Here is good example of how you can structure the files for a project:

.. code-block:: shell

	config.toml
	game.lua
	game.ico
	images/texture1.png
	sounds/hit1.wav

Make sure you place these files in a unique folder, not just on your desktop, or
in the documents folder. Keeping your files tidy, will make sure you don't run into
problems later down the road when you try to share your game with others.

|
|

The ``config.toml`` file
------------------------

When Xentu launches, it looks for a ``config.toml`` file to configure the game
window. The configuration is written using the TOML file format. Here is an example
of how one of these files looks:

.. code-block:: toml

	[general]
	game_title = 'Brick Breaker'
	screen_width = 1280
	screen_height = 720
	update_frequency = 60
	vsync = true
	fullscreen = false

	[viewport]
	width = 800
	height = 600
	mode = 1

For now the ``[general]`` and ``[viewport]`` groups are the only built-in ones
that have some rules. For example the engine will throw an error if you use anything
but a rational integer for screen_width or screen_height.

.. admonition:: The TOML file format.

	This format was chosen to allow you can provide	end users the ability to
	configure your game further, by reading custom set config values later in your
	game code.

|

Here is a list of commands that you can use in the ``[general]`` group:

+------------------+-----------+-----------------------------------------------+
| Key              | Data Type | Description                                   |
+==================+===========+===============================================+
| game_title       | string    | The title of the game window.                 |
+------------------+-----------+-----------------------------------------------+
| screen_width     | integer   | The width of the window in pixels.            |
+------------------+-----------+-----------------------------------------------+
| screen_height    | integer   | The height of the window in pixels.           |
+------------------+-----------+-----------------------------------------------+
| update_frequency | integer   | The target number of update calls per-second. |
+------------------+-----------+-----------------------------------------------+
| vsync            | boolean   | Enable or disable vertical sync.              |
+------------------+-----------+-----------------------------------------------+
| fullscreen       | boolean   | Enable or disable full-screen on load.        |
+------------------+-----------+-----------------------------------------------+

Here is a list of commands that you can use in the ``[viewport]`` group:

+------------------+-----------+-----------------------------------------------+
| Key              | Data Type | Description                                   |
+==================+===========+===============================================+
| width     	   | integer   | The width of the viewport in pixels.          |
+------------------+-----------+-----------------------------------------------+
| height           | integer   | The height of the viewport in pixels.         |
+------------------+-----------+-----------------------------------------------+
| mode             | integer   | The size mode of the viewport                 |
+------------------+-----------+-----------------------------------------------+

The viewport is the rendered canvas. If the viewport is smaller than the screen,
then black borders will appear. If the viewport is larger, then the rendered
game will be clipped.

Setting the mode decides where to place the viewport. The valid values are 0 for
normal (top left), 1 to place the viewport in the centre of the screen, or 2 to
stretch the viewport to all 4 corners.

|
|

The ``game.lua`` file
---------------------

When Xentu launches, it looks for a ``game.lua`` file to execute. Here is an
example of how this file could look:

.. code-block:: lua

	game.on("init", function()
	   -- load a tilemap
	   map = assets.load_tilemap("desert.tmx")
	   layer = map.get_layer(0)

	   -- set the background color.
	   renderer.set_clear_color( Color.fromHex('#5f7988') )
	end)


	-- the update event.
	game.on("update", function()
	   -- do nothing for now
	end)

	-- the drawing event.
	game.on("draw", function()
	   renderer.begin()
	   -- draw a layer [layer, x, y]
	   renderer.draw_tilemap_layer(layer, 0, 0)
	   renderer.present()
	end)

This example loads a TMX file created with the Tiled map editor. On the "init"
event, a reference to the first layer is created, then we ask the game to render
that layer over and over in the "draw" event.