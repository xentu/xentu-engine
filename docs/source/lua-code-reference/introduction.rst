============
Introduction
============

Xentu games are created using the Lua programming language. On load, the engine
looks for a file called ``game.lua``, and executes the code in that file
before starting the game.

From ``game.lua`` you can include code from other lua files using the include
command like so:

.. code-block:: lua

    include('another-script.lua')

You can also use the more traditional require function like so:

.. code-block:: lua

    require('path.to.another-script')

The difference between require and include, is that include uses the engines IO
system, which will have support in the future for embedded or network resources.
The require function on the other hand wont, but is preserved to give the ability
to load Lua modules.

|

Most common functions in lua (currently Lua v5.3) work, so for example to output
some text to the console you can use something like this:

.. code-block:: lua

    print('Hello From Lua!')

You also have access to the rest of the `standard libraries <https://www.lua.org/manual/5.3/manual.html#6>`_ 
including the math and coroutines features.

|
|