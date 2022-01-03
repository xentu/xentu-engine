================
The Color struct
================

The Color struct is used to specify and manipulate Color, it allows conversion
between values such as hexidecimal notation, alpha values and HSL. Note
that the color components are stored as floats.

|

Color(r, g, b, a)
-----------------

Use the main constructor to initialize a color from ARGB. Each component must be
between 0.0 and 1.0.

.. code-block:: lua
	
	local red = Color(1, 0, 0, 1)

|

Color.from_hex(hex)
-------------------

Use this method to construct a color struc using hexidecimal notation. The hex
value must start with a hashtag, and have 6 digits to be seen ad valid. Here is
an example of how to use it:

.. code-block:: lua
	
	local red = Color.from_hex('#FF0000')

Future version of the engine will allow 3,4 or 8 digits variants.

|

Color.from_hsl(h, s, l, a)
--------------------------

Use this method to construct a color struct using hue saturation, lightness and
alpha components. Each component must be between 0.0 and 1.0. For example:

.. code-block:: lua
	
	local red = Color.from_hsl(0, 1, 0.5, 1)