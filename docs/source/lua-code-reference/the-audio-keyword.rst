=================
The audio keyword
=================

This is the audio component of the game engine. Use it to play sounds, and music.
Upto 8 mixer tracks can be muxed at the same time thanks to the wonderful MiniAudio
library.

|
|

audio.is_playing(sound)
-----------------------

Test weather any instance of a specific loaded sound is currently playing.

.. code-block:: lua
	
	if audio.is_playing(sound1) then
	    print('sound1 is playing!')
	end

|

audio.play(sound)
-----------------

The play method begins playing a single instance of a loaded sound. 

.. code-block:: lua
	
    song1 = assets.load_audio('audio/song1.mp3')
    audio.play(song1)

|

audio.set_volume(sound)
-----------------------

This method sets the volume of all playing instances of a loaded sound. The value
you pass should be between 0.0 and 1.0.

.. code-block:: lua
	
    song1 = assets.load_audio('audio/song1.mp3')
    audio.set_volume(song1, 0.75)

|

audio.stop(sound)
-----------------

This method stops playing all instances of a loaded sound.

.. code-block:: lua
	
    audio.stop(song1)

|