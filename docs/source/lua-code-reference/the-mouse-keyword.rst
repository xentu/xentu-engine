=================
The mouse keyword
=================

A basic input component for reading signals from a plugged in mouse. 

|
|

mouse.mouse_x
-------------

This property tells you where the mouse cursor is on the X axis relative to the
top left of the game window.

|
|

mouse.mouse_y
-------------

This property tells you where the mouse cursor is on the Y axis relative to the
top left of the game window.

|
|

mouse.button_down(button_code)
------------------------------

This method tells you weather one of the mouse buttons are currently pressed. The
valid values include MOUSE_LEFT, MOUSE_MIDDLE and MOUSE_RIGHT. Here is how to use
it:

.. code-block:: lua
	
	if mouse.button_down(MOUSE_LEFT) then
	    print('The left mouse button is pressed.')
	end