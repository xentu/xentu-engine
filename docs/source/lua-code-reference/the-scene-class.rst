===============
The Scene class
===============

The scene class wraps together game logic, asset loading draw calls & properties
that all belong to one part of your game. A scene could be a menu screen, or a
single game level, or a score board, or an intro video.

For an example on how to use the scene class in your game. See :ref:`game.set_scene(scene)`

|
|

Scene(name)
-----------

This is a constructor function used for creating new scenes. Use it like this:

.. code-block:: lua

	local scene = Scene("My Scene")

The first argument is optional, and is the display name of the scene, not to be 
confused with *scene* which is the variable name.

.. note::

	The examples below use ``scene...`` to demonstrate various techniques. However
	this assumes you have created a "scene" variable like the one above.

|
|

scene.name
----------

This property lets you get or set an optional display name of the scene. 

|
|

scene.on(event)
---------------
The ``on`` method allows you to subscribe to a named built-in or custom event. The
built in events are really useful as these allow you to hook in and control
scene game logic, drawing etc... 

Here are some code examples for key events you should be aware of:

The init event
^^^^^^^^^^^^^^

.. code-block:: lua

	scene.on('init', function()
	    -- do something here
	end)

The ``init`` event is called when the scene is ready to start loading assets for
your scene. It can only be fired once, and it's called when ``game.set_scene`` is
used, however you can also trigger the event manually by using ``[instance].trigger('init')``.

The shown event
^^^^^^^^^^^^^^^

.. code-block:: lua

	scene.on('shown', function(dt)
	    -- do something here
	end)

As a scene can be shown more than once, an event called shown is made available
to allow you to handle what happens when a specific scene is selected as the
active one. If a previous scene was active, a *hidden* event is fired on the
previous scene before hand.

The hidden event
^^^^^^^^^^^^^^^^

.. code-block:: lua

	scene.on('hidden', function(dt)
	    -- do something here
	end)

The scene system also provides an event to let a scene know it has lost focus.
This event is called before the *shown* event.

The update event
^^^^^^^^^^^^^^^^

.. code-block:: lua

	scene.on('update', function(dt)
	    -- do something here
	end)

The ``update`` event is called to give your scene the opportunity to update game
logic. This callback provides a dt argument which is a float representing the
delta time since update was last called, useful for timing animations, or game logic.

The draw event
^^^^^^^^^^^^^^

.. code-block:: lua
	
	scene.on('draw', function()
	    renderer.begin()
	    -- do something here
	    renderer.present()
	end)

The ``draw`` event is called to give your scene the opportunity to draw graphics
specific to the scene.

|
|

scene.trigger(event)
--------------------

Trigger allows you to trigger custom events throughout your code. These events
can be handled by the ``on`` method shown above. To trigger an event, simply write
something like this:

.. code-block:: lua

    scene.trigger('my_custom_event')

The first argument is the name of the event to trigger. The base trigger
method does not support passing additional arguments.