=======================
The TileMapObject class
=======================

A TileMapObject provides information for a single object loaded along side 
a :ref:`TileMapLayer <The TileMapLayer class>` instance. Objects tend to be used
for entities, polygons (lines, areas, triggers).

|
|

[instance].name
---------------

This property gives you the name of the object.

|
|

[instance].shape
----------------

This property gives you information about what the object is used for. Usual values
include Rectangle, Ellipse, Point, Polygon, Polyline & Text.

|
|

[instance].x
------------

Use x coordinate for the bound of this object.

|
|
 
[instance].y
------------

Use y coordinate for the bound of this object.

|
|

[instance].width
----------------

Use width for the bound of this object.

|
|

[instance].height
-----------------

Use height for the bound of this object.

|
|

[instance].point_count
----------------------

Get or set the number of polygonal points loaded into this object. This
is stored as an unsigned integer.

|
|

[instance].get_point(point_index)
---------------------------------

Use this function to get an individual polygonal point from an
object. The single argument point_index expects a value between
0 and point_count. Two float values are returned (x, y) which are
both relative to the object position, so to get the real position
you will need to do a little math. Here is a code example:

.. code-block:: lua

	-- get the layer offset
	offset_x, offset_y = layer1.offset

	-- get an object
	local obj1 = layer1.get_object(0)

	-- get the x and y of the object's first point
	local x, y = obj1.get_point(0)

	-- add the x and y from the object
	x = x + obj1.x
	y = y + obj1.y

	-- add the layer coord offset to get the true render coord
	x = x + offset_x
	y = y + offset_y

The above takes a little getting used to, however this gives you
great power to move things about, as you can modify coordinates
either at point, object or layer level, and everything below will
adjust accordingly.

|
|

[instance].set_point(point_index, x, y)
---------------------------------------

Use this function to set an individual polygonal point on an
object. The first argument point_index expects a value between
0 and point_count. Here is an exmaple:

.. code-block:: lua

	obj1.set_point(1, 50.0, 37.0)

If you wish to add more points than point_count permits, change
the point_count first adding however many points you need, then
call this function like this:

.. code-block:: lua

	obj1.point_count = 3
	obj1.set_point(2, 55.0, 42.0)