=================
The TileMap class
=================

A TileMap is a complex object made up of tile set’s (similar to sprite map’s), 
properties, and a collection of layers of either objects or arranged tiles that
can be drawn onto the screen.

This type of object is ideal for building complicated visuals in games. Things
like levels for platformers, or tile based games (think Stardew Valley). Xentu
currently supports the TMX format, which is provided by a popular editor for 
these kinds of structures called Tiled.

When a TileMap instance is loaded in Xentu, any textures and layers are also
loaded with it automatically. If all you wish to do is draw what you've loaded, 
here is an example of how to do that:

.. code-block:: lua

	game.on('init', function()
	    -- load the TileMap
	    map = assets.load_tilemap("level1.tmx")
	    -- get one of the layers by index
	    layer0 = map.get_layer(0)
	end)

	game.on('draw', function()
	    renderer.begin()
	    renderer.draw_tilemap_layer(layer0)
	    renderer.present()
	end)

Please note you must draw each layer individually, there is no draw_tilemap method.

For more information what information each layer you can gain access to provides,
please see the :ref:`The TileMapLayer class`.
