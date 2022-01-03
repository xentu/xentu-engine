=================
The Sprite struct
=================

The sprite struct is a basic structure that can be used in Xentu to draw all or
part of a texture onto the screen. All you need to get started is a loaded texture,
and an idea of where you want to draw it.

|

Sprite(texture, x, y, width, height)
------------------------------------

The main constructor is used to create a sprite instance. For example:

.. code-block:: lua
	
	ball_texture = assets.load_texture('ball.png', TX_RGBA, TX_REPEAT)
	ball_sprite = Sprite(ball_texture, 0, 0, 100, 100)

Then in your render event you could draw it like this:

.. code-block:: lua

	-- the drawing event.
	game.on("draw", function()
	    renderer.begin()
	    renderer.draw_sprite(ball_sprite)
	    renderer.present()
	end)

|

[instance].spritemap
--------------------

If you wish to render only a region of the texture you've picked. You can use a
loaded spritemap:

.. code-block:: lua

	ball_sprite.spritemap = assets.load_spritemap('ball_sprites.json')

|

[instance].region
-----------------

Along with the spritemap property, you can also choose which sprite on the spritemap
to use by providing a region name. By default this is set to 'full', however you can
change it:

.. code-block:: lua

	ball_sprite.region = "walk1"

This can be really useful for providing some basic animation for your game sprites.

|

[instance].x
------------
 
The x property can be modified to move the sprite on the x-axis like so:

.. code-block:: lua
    
	ball_sprite.x = ball_sprite.x + 10

|

[instance].y
------------
 
The y property can be modified to move the sprite on the y-axis like so:

.. code-block:: lua
    
	ball_sprite.y = ball_sprite.y + 10

|

[instance].width
----------------
 
The width property can be modified to change the width of the drawn sprite:

.. code-block:: lua
    
	ball_sprite.width = 200

|

[instance].height
-----------------
 
The height property can be modified to change the height of the drawn sprite:

.. code-block:: lua
    
	 ball_sprite.height = 200