==================
The config keyword
==================

A utility for getting configuration variables stored in config.toml

|
|

config.get_string(group_and_key)
--------------------------------

This method is used to get a string value from the loaded config. Here is how to
use it:

.. code-block:: lua
	
	local game_title = config.get_string('general.game_title')
	print("This game is called " .. game_title)

|

config.get_integer(group_and_key)
---------------------------------

This method is used to get a number value from the loaded config, this can also
be used to get boolean values. Here is how to use it:

.. code-block:: lua

	local screen_width = config.get_integer('general.screen_width')
	print("This game screen width is " .. screen_width)