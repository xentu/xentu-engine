===============
Coding Concepts
===============

For the most part, Xentu is an environment for running code written in the Lua
programming language. However if you wish to interact with the systems exposed 
to make a computer game, there are some concepts and conventions you should
understand.

In this section we will go over what each of those concepts are so that you can
take full advantage of the engine.

|
|

Object Types
------------

Xentu encourages OOP (object-orientated programming) where possible. To facilitate
this, an event system is implemented on the game and scene subsystems. And a set
of object types are exposed.

|

Keyword Object Type
^^^^^^^^^^^^^^^^^^^
The main core components of the Xentu game engine are exposed as already existing
global variables which we call *keywords*. The keywords never change, and provide
static methods and properties which should be accessed using period notation. For
example, the :ref:`game <the game keyword>` keyword exposes a method called *trigger* and has 1 argument
called name. We would interact with this in Lua like this:

.. code-block:: lua

	game.trigger("my_event")

Other keywords include audio, config, keyboard, mouse, viewport and renderer.

|

Class Object Type
^^^^^^^^^^^^^^^^^
When the engine is asked to natively load a complex data object such as a :ref:`TileMap <the TileMap class>`
or say a :ref:`SpriteMap <the SpriteMap class>`, Xentu creates a native C++ instance, then exposes it to Lua using
something called Luna. This is mostly done for the keywords as explained above,
which allows the engine to run really fast.

There is an exception to the rule. Xentu allows you to create new instances of :ref:`the
Scene class <the Scene class>`, which allows the engine to wrap it's fast rendering and update system
around chunks of your code. 

|

Struct Object Type
^^^^^^^^^^^^^^^^^^
Structs (or structures) in Xentu are a built in system for creating smart objects. These
objects are native to Lua, can have a constructor, inherited properties, functions and
are a great way to manage things like entities.

Here is an example of how the Font struct used for rendering text was created:

.. code-block:: lua

	-- A class that holds information about a font that can be used to draw text.
	Font = struct(function(inst, texture, spritemap) 
	    if not (type(texture) == "number") then error("Invalid value for texture.") end
	    if not (type(spritemap) == "number") then error("Invalid value for spritemap.") end
	    inst.texture = texture
	    inst.spritemap = spritemap
	    inst.letter_spacing = 1
	    inst.line_height = 20
	end)

You don't have to use the struct system, it's there to help keep your code organised and
easy to manage.

|

Asset Object Type
^^^^^^^^^^^^^^^^^
When you load a texture or a sound using the `asset <the asset keyword>` keyword, the value
returned is actually an integer. Xentu uses a basic reference counting/accessor system to
keep track of memory allocated. The integer value is the count -1 of the specific loaded asset
index.

So if you loaded 2 textures, you would likely get 1 and 2 back (0 usually is reserved for 
the blank default texture). If you then loaded a sound, the returned value would start again
from 0.

So when you load a texture, assign that texture to a sprite, then draw the sprite, the 
engine actually just passes an integer around to find the right texture to draw.

.. note::

	The integer returned when loading a texture is not the id you get from creating a texture
	in OpenGL. These integers might align by accident, however the integer you get is specific
	to the index in a vector stored in C++

You may find several uses fo

|
|