=============
Using The SDK
=============

The SDK (software development kit) installation consists of the following file
structure:

+------------------+-----------------------------------------------------------+
| File / Folder    | Description                                               |
+==================+===========================================================+
| ``xentu.exe``    | The game engine itself, which should be distributed with  |
|                  | your game code when you share the games you create.       |
+------------------+-----------------------------------------------------------+
| ``xentusdk.exe`` | A companion app that provides commands a Xentu developer  |
|                  | will need. Things such as changing to a different version |
|                  |                                                           |
|                  | of the engine. Or packaging a game you've made for a      |
|                  | specific platform etc...                                  |
+------------------+-----------------------------------------------------------+
| ``logo.png``     | The Xentu logo icon, can be used to test texture loading. |
+------------------+-----------------------------------------------------------+
| ``templates/``   | A folder containing a number of demo games contained      |
|                  | within ZIP archives.                                      |
+------------------+-----------------------------------------------------------+
| ``glfw3.dll``    | [windows only] A copy of the GLFW3 OpenGL Library (32bit).|
+------------------+-----------------------------------------------------------+
| ``glew32.dll``   | [windows only] A copy of the GLEW Window Library (32bit). |
+------------------+-----------------------------------------------------------+

Some other files may also appear depending on what method you chose to install
the engine.

For Linux/FreeBSD and MacOS expect the .exe to be dropped from binaries.