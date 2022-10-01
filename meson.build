project('xentu_rebuild', 'c', 'cpp',
        default_options: ['default_library=static'])

glew = dependency('glew')
ogg_dep = dependency('ogg')
vorbis_dep = dependency('vorbis')
sdl2_image_dep = dependency('SDL2_image')
sdl2_mixer_dep = dependency('sdl2_mixer')
sdl2_ttf_dep = dependency('sdl2_ttf')
sdl2_dep = dependency('sdl2')
py_dep = dependency('python3')
lua_dep = dependency('lua-5.4')
incdir = include_directories(['lib'])


project_source_files = files([
  # libs
  'lib/ducktape/duktape.c',
  'lib/miniz/miniz.c',

  # assets
  'src/assets/AssetManager.cpp',

  # audio
  'src/audio/AudioManager.cpp',

  # filesystem
  'src/vfs/XenFileInfo.cpp',
  'src/vfs/XenMemoryFile.cpp',
  'src/vfs/XenMemoryFileSystem.cpp',
  'src/vfs/XenNativeFile.cpp',
  'src/vfs/XenNativeFileSystem.cpp',
  'src/vfs/XenStringUtils.cpp',
  'src/vfs/XenVirtualFileSystem.cpp',
  'src/vfs/XenZipFile.cpp',
  'src/vfs/XenZipFileSystem.cpp',

  # machines
  'src/machines/JavaScriptMachineInterop.cpp',
  'src/machines/JavaScriptMachine.cpp',
  'src/machines/LuaMachineInterop.cpp',
  'src/machines/LuaMachine.cpp',
  'src/machines/PythonMachineInterop.cpp',
  'src/machines/PythonMachine.cpp',
  'src/Machine.cpp',

  # graphics
  'src/Renderer.cpp',
  'src/graphics/Rect.cpp',
  'src/graphics/Sprite.cpp',
  'src/graphics/SpriteMap.cpp',
  'src/graphics/TextBox.cpp',
  'src/graphics/Texture.cpp',
  'src/graphics/Transform.cpp',
  'src/graphics/Transformable.cpp',
  'src/graphics/Vertex.cpp',
  'src/graphics/Viewport.cpp',

  #input
  'src/input/InputManager.cpp',

  # other
  'src/Globals.cpp',
  'src/Config.cpp',

  # main
  'main.cpp'
])


executable('xentu', project_source_files,
           include_directories : incdir,
           dependencies: [
              glew, sdl2_image_dep, sdl2_mixer_dep, ogg_dep, vorbis_dep,
              sdl2_ttf_dep, sdl2_dep, py_dep, lua_dep
            ])
           
           # win_subsystem: 'windows',