======================
The TileMapLayer class
======================

A TileMapLayer provides information for a single layer loaded along side 
a :ref:`TileMap <The TileMap class>` instance. Layers can have one or more roles.
Some have a single big texture (usually used for backgrounds), some have a collection
of tiles that can be rendered. Some can also host a collection of TileMapObject
instances which tend to be used for entities, polygons (lines, areas, triggers).

Each layer can also have meta information, such as a name, opacity/visibility, and
custom properties to allow you to add any extra information you need to pass to
the game.

|


[instance].name
---------------

Use this property to get or set the name of the layer.

|
 
[instance].opacity
------------------

Use this property to get or set the scalar opacity of the layer used for rendering.

|
 
[instance].type
---------------

This read-only property gives you the purpose of the layer. Most common values areas
Group, Image, Object and Tile.

|
 
[instance].visible
------------------

Use this property to get or set weather the layer is visible when rendering.

|
 
[instance].object_count
-----------------------

This read-only property tells you how many objects the layer has loaded.

|
|

[instance].get_offset()
-----------------------

This function tells you the x,y coordinate offset of the layer used when
rendering. Note that this function returns 2 values, example:

.. code-block:: lua

	local layer1 = map.get_layer(0)
	offsetx, offsety = layer1.offset

|
 
[instance].get_size()
---------------------

This function gives you the layer size in pixels (also returns 2 values).

.. code-block:: lua

	local layer1 = map.get_layer(0)
	width, height = layer1.size

|

[instance].get_object(index)
----------------------------

This method returns a :ref:`TileMapObject <The TileMapObject class>` based on the index you provide. Because you
need an index, it's reccommended that you read the object_count property first
before calling this to make sure your index is within bounds.

|
 
[instance].get_property_as_string(name)
---------------------------------------

This method gets the value of a custom property based on the property name. The
value returned if found will be a string.

.. code-block:: lua

	local prop1 = layer1.get_property_as_string("my_custom_property")
	print("prop1=" .. prop1)

|
 
[instance].get_property_as_int(name)
------------------------------------

This method gets the value of a custom property based on the property name. The
value returned if found will be an integer.

|
 
[instance].get_property_as_float(name)
--------------------------------------

This method gets the value of a custom property based on the property name. The
value returned if found will be a float.
