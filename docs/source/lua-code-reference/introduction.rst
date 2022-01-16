============
Introduction
============

Xentu games are created using the Lua programming language. On load, the engine
looks for a file called ``game.lua``, and executes the code in that file
before starting the game.

From ``game.lua`` you can require code from other lua files using the require
command like so:

.. code-block:: lua

    require('another-script.lua')

.. note::

	Note: The traditional *require* function has been renamed to *include* as
	it does not handle working within an archive, or over a network. Also the
	new require function does not yet work with modules.

Most common functions in lua (currently Lua v5.3) work, so for example to output
some text to the console you can use something like this:

.. code-block:: lua

    print('Hello From Lua!')

You also have access to the rest of the `standard libraries <https://www.lua.org/manual/5.3/manual.html#6>`_ 
including the math and coroutines features.

|
|