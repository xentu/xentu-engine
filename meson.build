
project('xentu_rebuild', 'c', 'cpp',
        version : run_command('cat', './version.txt').stdout().strip(),
        default_options: ['default_library=static', 'buildtype=debug']
        )

the_version = run_command('cat', './version.txt').stdout().strip()


glew = dependency('glew')
ogg_dep = dependency('ogg')
vorbis_dep = dependency('vorbis')
sdl2_image_dep = dependency('SDL2_image')
sdl2_mixer_dep = dependency('SDL2_mixer')
sdl2_ttf_dep = dependency('SDL2_ttf')
sdl2_dep = dependency('sdl2')
if host_machine.system() == 'linux'
  lua_dep = dependency('lua-5.3')
else
  lua_dep = dependency('lua-5.4')
endif
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
  'src/vfs/XenVirtualFileSystem.cpp',
  'src/vfs/XenZipFile.cpp',
  'src/vfs/XenZipFileSystem.cpp',

  # machines
  'src/machines/JavaScriptMachineInterop.cpp',
  'src/machines/JavaScriptMachine.cpp',
  'src/machines/LuaMachineInterop.cpp',
  'src/machines/LuaMachine.cpp',
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
  'src/StringUtils.cpp',

  # main
  'main.cpp'
])


dependencies = [
  glew, sdl2_image_dep, sdl2_mixer_dep, ogg_dep, vorbis_dep,
  sdl2_ttf_dep, sdl2_dep, lua_dep
]


conf_data = configuration_data()
conf_data.set('version', the_version)


if host_machine.system() == 'windows'
    project_source_files += import('windows').compile_resources( 'xentu.rc' )
endif


executable('xentu_debug', project_source_files, include_directories:incdir, 
           dependencies:dependencies, gui_app:false, 
           cpp_args : ['-DXEN_DEBUG','-DXEN_VER="' + the_version + '"'])

executable('xentu', project_source_files, include_directories:incdir,
           dependencies:dependencies, gui_app:true,
           cpp_args : ['-DXEN_VER="' + the_version + '"'])