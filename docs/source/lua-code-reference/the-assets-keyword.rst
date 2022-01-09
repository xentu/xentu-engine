==================
The assets keyword
==================

The assets keywords provides methods for loading game assets such as textures
and sounds. The filename structure can be either absolute, or relative (comared
to the main game.lua).

|
|

assets.load_audio(file)
--------------------------

Use this method to load audio files. The file formats supported by this method
are WAV, MP3 and FLAC. For best results encode in 44,000kHz. The result of this
method is a sound_id integer reference that should be stored somewhere so that
you can use it elsewhere. Here is an eample of how to use this method:

.. code-block:: lua

    sound1 = assets.load_audio('audio/track1.mp3')

In future versions the ability to load other sample rates will be implemented.

|
|

assets.load_shader(vert_file, frag_file)
----------------------------------------

Use this method to load a a shader program. You should pass it a filename for a
vertex shader, and another for a fragment shader. The result of this method is an
integer specifying the id of the shader. Here's an example of how to do it:

.. code-block:: lua

    shader1 = assets.load_shader('shaders/vertex.glsl', 'shaders/fragment.glsl')    

Xentu supports shaders written in GSLS compatible with OpenGL ES 3.2. Lets take
a look at how a basic monochrome shader filter could be written. First the vertex
shader:

.. code-block:: glsl

    #version 410

    in vec3 i_position;
    in vec2 i_texcoord;
    in vec4 i_color;

    uniform mat4 u_MVP;

    out vec2 v_TexCoord;
    out vec4 v_Color;

    void main()
    {
        gl_Position = u_MVP * vec4(i_position, 1.0);
        v_TexCoord = i_texcoord;
        v_Color = i_color;
    }

Next the fragment shader:

.. code-block:: glsl

    #version 410

    in vec2 v_TexCoord;
    in vec4 v_Color;

    uniform sampler2D u_Texture;

    out vec4 frag_colour;

    void main()
    {
        vec4 color = texture(u_Texture, v_TexCoord) * v_Color;
        frag_colour = vec4(color.r, color.r, color.r, color.a);
    }

You can not currently pass extra information into a shader, however this is something
that will be added in a very near future build.

|
|

assets.load_texture(texture_file, color_mode, wrap_mode)
--------------------------------------------------------

Use this method to load a texture. This method allows you to specify color mode,
and a wrap mode (of which both arguments are optional). The file formats supported
by this method are the same that are supported by the stb_image library (JPG, PNG,
TGA, BMP, GIF, HDR). The result of this method is an integer indicating the
texture_id that should be stored somewhere so that you can use it elsewhere. Here
is an example of how to use the method:

.. code-block:: lua

    texture1 = assets.load_texture('texture1.png', TX_RGBA, TX_REPEAT)

The currently available color modes are:

- **TX_RED** - For 8bit red channel only.
- **TX_RGB** - For 8bit RGB colour without an alpha channel.
- **TX_RGBA** - For 8bit RGB colour with an alpha channel.

The currently available wrap modes are:

- **TX_REPEAT** - The default mode.
- **TX_CLAMP_TO_EDGE** - A mode of clamping to the edge resulting in a stretched edge pattern.
- **TX_CLAMP_TO_BORDER** - A mode force clamping the image to the border.

More modes (such as compressed variants of color modes) will become available as
development on the engine progresses. Feel free to make a feature request if one
you need is currently not provided.

|
|

assets.load_spritemap(json_file, format)
----------------------------------------

A SpriteMap is a loaded resource used as an information source for the Font and
Sprite structs. It provides a map for a texture letting the engine know where 
named regions are. This method allows you to load those files from various
compatible formats. Here is how to use it:

.. code-block:: lua

	spriteMap1 = assets.load_spritemap('texture1_sprites.json', SM_DEFAULT)

The second argument is optional and specifies the format. Here is a list of format codes that you can provide it:

- **SM_DEFAULT** - The built in JSON format, created using a not yet released SpriteMap tool.
- **SM_ASESPRITE** - Reads a spritemap from a JSON file genereated by the app Aseeprite.
- **SM_TEXPACK** - Reads a spritemap from a JSON file genereated by the app TexturePacker.

For more information about how to use a sprite map once loaded, please see this page.

|
|

assets.load_tilemap(tmx_file)
-----------------------------

A tile map is a complex object made up of tile set's (similar to sprite map's),
properties, and a collection of layers of either objects or arranged tiles that can
be drawn onto the screen.

Here is an example of how to load a tilemap:

.. code-block:: lua

	tilemap = assets.load_tilemap('level1.tmx')

When you call this function the tile map will be loaded, along with any associated
tile set's and textures. For more information check out :ref:`The TileMap Class`