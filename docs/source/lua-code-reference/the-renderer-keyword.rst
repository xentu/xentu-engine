=====================
The renderer keyword.
=====================

The renderer is the main utility in Xentu used for drawing graphics onto the
screen. In terms of functionality it is very similar to the SpriteBatch that
you'll find in Microsoft XNA and MonoGame.

At it's core, the renderer groups organised batches of sprite calls together,
and sends them to the GPU to be rendered. Doing this allows the engine to draw
many graphics at once at a very fast rate.

Typically the renderer is used within the *draw* event in your ``game.lua``
file. Here is an example of how you could use it:

.. code-block:: lua
	
	game.on('draw', function(arg)
		renderer.begin()
		renderer.draw_sprite( my_sprite )
		renderer.draw_sprite( my_sprite2 )
		renderer.present()
	end)

Notice that we use a ``begin()`` and ``present()`` call surrounding the actual
call to draw two sprites. This group constitutes as a single drawing layer. As
long as you finish a layer with the ``present()`` call, you can begin a new one 
straight after like so:

.. code-block:: lua
	
	game.on('draw', function(arg)
		-- layer #1
		renderer.begin()
		renderer.draw_sprite( my_sprite )
		renderer.draw_sprite( my_sprite2 )
		renderer.present()

		-- layer #2
		renderer.begin()
		renderer.draw_sprite( my_sprite3 )
		renderer.present()
	end)

The above may not seem that useful at first. However once you start building your
own games, you will understand why this can be so powerful.

|

Basic drawing
=============

renderer.begin()
----------------

The begin method tells the renderer to clear it's instruction buffer, and prepare
for new draw instructions. If you dont do this after calling ``present()``. The
result from the previous draw call will still be stored in the instruction buffer.

|

renderer.present()
------------------

The present method tells the renderer to send all queued instructions from the
instruction buffer to the GPU for rendering.

|

renderer.draw_sprite(sprite)
----------------------------

This method tells the renderer to queue an instruction in it's buffer to draw a
sprite. The sprite argument must be an instance of the :ref:`Color <The Color Class>` class setup
earlier in your code.

|

renderer.draw_text(font, text, x, y, max_width)
-----------------------------------------------

This method tells the renderer to queue an instruction in it's buffer to draw 
text onto the screen. The method requires you provide an instance to a :ref:`Font <The Font Class>`
class, some text, an x-y position, and a maximum width before wrapping onto the
next line.

|

Blending graphics
=================

renderer.set_blend(bool)
---------------------------

The set_blend method is used to specify weather or not to use alpha blending when
rendering. By default it is set to true, however you can switch it off if needed.

|

renderer.set_blend_func(sfactor, dfactor)
-----------------------------------------

This method allows you to customise how the blending of graphics works. ``sfactor`` is
the source factor, and ``dfactor`` is the destination factor. The available
values for each argument are as follows:

- ZERO
- ONE
- SRC_COLOR	
- ONE_MINUS_SRC_COLOR
- DST_COLOR	
- ONE_MINUS_DST_COLOR
- SRC_ALPHA	
- ONE_MINUS_SRC_ALPHA
- DST_ALPHA	
- ONE_MINUS_DST_ALPHA
- CONSTANT_COLOR
- ONE_MINUS_CONSTANT_COLOR
- SRC_ALPHA_SATURATE
- SRC1_COLOR
- ONE_MINUS_SRC1_COLOR
- SRC1_ALPHA
- ONE_MINUS_SRC1_ALPHA

A new method is currently in the works called ``set_blend_preset()`` that will
instead only require more commonly known blend terms as described by the Mozilla
foundation's documentation `here <https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/globalCompositeOperation>`_.
This should be arriving over the next few weeks.

|

renderer.set_clear_color(color)
-------------------------------

This method allows you to pick a color to clear the background with just before
``present()`` sends new drawing calls to the GPU. The color argument requires 
creating a new instance of the :ref:`Color <The Color Class>` class. Here is an example of how to
use it:

.. code-block:: lua
	
	local blue = Color.fromHex('#0000ff')
	renderer.set_clear_color(blue)

|

Transforming Coordinates
========================

Xentu uses a matrices multiplication transform system, this means if you set a
rotation, then a translation, then another rotation, you maybe confused with the
result. Think of it as if each transform call you make is an add operation, not
an overwrite.

Every time you call the ``begin()`` method, the transforms you setup will be reset.

|

renderer.set_origin(x, y)
-------------------------

This method sets the transform origin for everything drawn after it. The origin
typically starts as 0,0 (top left) meaning if you draw a sprite without any
transformation. It'll appear in the top left without any clipping.

Imagine that sprite is 100x100 pixels in size, and you call ``set_origin(50, 50)``.
This will move the origin to exactly the centre of that sprite. So when you draw
it, you'll instead only see the bottom right quater of the sprite.

|

renderer.set_rotation(angle)
----------------------------

This method transforms the next rendered graphic by a angle based rotation. If we
expand on the ``set_origin()`` example from above, using ``set_rotation(45)``
would effectively rotate the sprite around it's centre by 45 degrees.

|

renderer.set_scale(x, y)
------------------------

This method transforms the next rendered graphic by scale. So if you wrote the
following:

.. code-block:: lua
	
	renderer.set_scale(2, 2)

The next drawn graphic would be 200% tall, and 200% wide.

|

renderer.set_translate(x, y)
----------------------------

This method transforms the next rendered graphic by moving (translating). So if
you wrote the following:

.. code-block:: lua
	
	renderer.set_translate(10, 2)

The next drawn graphic would move by 10 pixels to the right.

|

Using Shaders
=============

renderer.set_shader(shader)
---------------------------

The ``set_shader()`` method allows you to tell the renderer what shader program
to use when ``present()`` is called. The shader argument should be an instance
of the :ref:`Shader <The Shader Class>` class created earlier in your code.

.. note:

	Uniform values that you provide to the Shader instance are only applied when
	``present()`` is called. Bear this in mind if you intend on calling ``present()``
	multiple times.