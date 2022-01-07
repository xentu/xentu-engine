===============
The Font struct
===============

The font struct wraps information to pass to the renderer when drawing text. It
includes useful things like which loaded sprite font to use, letter/line spacing
etc...

|
|

Font(texture, spritemap)
------------------------

The main constructure requires that you pass it a texture and spritemap instance,
here is an example of how you'd load those then use this constructor:

.. code-block:: lua
	
	texture = assets.load_texture("arial_20.png", TX_RGBA, TX_CLAMP_TO_BORDER)
	spritemap = assets.load_spritemap("arial_20.json")
    font = Font(texture, spritemap)

|
|

[instance].letter_spacing
-------------------------

Once you have created a font struct instance, the letter_spacing property can
be set like so:

.. code-block:: lua

    font.letter_spacing = 1

Letter spacing determines how many pixels are placed on the x-axis between each
text character.

|
|

[instance].line_spacing
-------------------------

With a font struct instance, the line_spacing property can also be set like so:

.. code-block:: lua
	
	font.line_spacing = 24

Line spacing determines how many pixels are used to space lines ot text on the
y-axis.