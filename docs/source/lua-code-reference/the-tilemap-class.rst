=================
The TileMap class
=================

The TileMap class contains the data loaded from a TMX file. If you wish to load
in complicated levels or designs for your game, this is a great way to do that.

When a TileMap instance is created, any textures and layers are also loaded with
it automatically. If all you wish to do is draw what you've loaded, here is an
example of how to do that:

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