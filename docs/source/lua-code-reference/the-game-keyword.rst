================
The game keyword
================

The game keywords is in charge of managing game events, controlling the 
window that hosts the game, and other generic tasks like exiting or restarting
etc...

|
|

game.debug_stack()
------------------
This method is used to get a glimpse of what is stored in the lua stack. For The
most part this method is really only useful for debugging niche situations.

|

game.exit()
-----------
Call this method to immediately exit the game.

|

game.fullscreen(bool)
---------------------
Use this method to change the screen to either full screen or windowed mode.

.. note::

	Calling this method too often can crash the game, be careful not to call it
	more than once in quick succession.

|

game.on(event)
--------------
The ``on`` method allows you to subscribe to a named built-in or custom event. The
built in events are really useful as these allow you to hook in and control
game logic, drawing etc... The ``on`` event is available on the game global, and
instances of the Scene class.

Here are some code examples for key events you should be aware of:

The init event
^^^^^^^^^^^^^^

.. code-block:: lua

	game.on('init', function()
	    -- do something here
	end)

The ``init`` event is called when the game is ready to start loading assets For
your game. At this point the game window has launched, and the sound system has
initialized.

The update event
^^^^^^^^^^^^^^^^

.. code-block:: lua

	game.on('update', function(dt)
	    -- do something here
	end)

The ``update`` event is called when the game wants to update game logic. This
event is triggered using a separate frequency set by the update_frequency config
value (usually set to 30 times a second). This callback provides a dt argument
which is a float representing the delta time since update was last called, useful
for timing animations, or game logic.

The draw event
^^^^^^^^^^^^^^

.. code-block:: lua
	
	game.on('draw', function()
	    renderer.begin()
	    -- do something here
	    renderer.present()
	end)

The ``draw`` event is called when the game wants to draw a frame. This event is
usually combined with calls to the renderer keyword, as that is the main system
for rendering graphics and text onto the screen.

|

game.set_scene(scene)
---------------------
By default Xentu fires the ``update`` and ``draw`` events through game object
global. However you can alternatively tell the game to fire ``update``
and ``draw`` events attached to scene objects that you create with ``new_scene()``.
Here is an example of how it would look:

.. code-block:: lua

	scene1 = new_scene()
	game.set_scene(scene1)

	scene1.on('update', function()
	    -- update code for scene goes here.
	end)

	scene1.on('draw', function()
	    -- draw code for scene goes here.
	end)

This functionality allows you to switch between sets of update/render calls, giving
a convenient way to switch between parts of a game. For example you could have a
scene exclusively for the menu, one for the main gameplay, and perhaps one for a
scoreboard.

Passing ``nil`` to this function unattaches the currently attached scene.

.. note::

	Even when a scene is attached using this method, the ``update`` and ``draw``
	calls on the main game object global will still be called. This is important
	as it's one of the mechanisms you can use to perform fades between scenes,
	or the changing of music etc...

|

game.trigger(event)
------------------------

Trigger allows you to trigger custom events throughout your code. These events
can be handled by the ``on`` method shown above. To trigger an event, simply write
something like this:

.. code-block:: lua

    game.trigger('my_custom_event')

The first argument is the name of the event to trigger. At this time the trigger
method does not support passing additional arguments.