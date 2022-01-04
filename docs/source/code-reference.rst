==============
Code Reference
==============

Xentu games are created using the Lua programming language. On load, the engine
looks for a file called ``game.lua``, and executes the code in that file
before starting the game.

From ``game.lua`` you can require other lua files using the require command like
so:

.. code-block:: lua

    require('another-script.lua')

Most common functions in lua (currently Lua v5.3) work, so for example to output
some text to the console you can use something like this:

.. code-block:: lua

    print('Hello From Lua!')

|

Engine Keywords
===============

Xentu provides various *keywords* to Lua which act as your way of communicating
and controlling the behavior of the engine. In this section we will look at what
each of the keywords are provided, and how to use them.

The following sections are in order of importance:

.. toctree::
    :titlesonly:
    
    lua-code-reference/the-game-keyword
    lua-code-reference/the-assets-keyword
    lua-code-reference/the-renderer-keyword
    lua-code-reference/the-audio-keyword
    lua-code-reference/the-keyboard-keyword
    lua-code-reference/the-mouse-keyword
    lua-code-reference/the-viewport-keyword
    lua-code-reference/the-config-keyword
    
    lua-code-reference/the-color-struct
    lua-code-reference/the-font-struct
    lua-code-reference/the-scene-class
    lua-code-reference/the-shader-class
    lua-code-reference/the-sprite-struct
    lua-code-reference/the-tilemap-class
    lua-code-reference/the-tilemaplayer-class
    lua-code-reference/the-tilemapobject-class