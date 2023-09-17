project('xentu_rebuild', 'c', 'cpp',
        version : run_command('cat', './version.txt').stdout().strip(),
        default_options: ['default_library=static', 'buildtype=release', 'cpp_std=c++17']
        )


the_version = meson.project_version()

glew = dependency('glew')
ogg_dep = dependency('ogg')
vorbis_dep = dependency('vorbis')
sdl2_image_dep = dependency('SDL2_image')
sdl2_mixer_dep = dependency('SDL2_mixer')
sdl2_ttf_dep = dependency('SDL2_ttf')
sdl2_dep = dependency('sdl2')

cmake = import('cmake')
if not sdl2_dep.found()
	cmake_opt = cmake.subproject_options()
	cmake_opt.add_cmake_defines({'LIBC': true})
	sdl2_proj = cmake.subproject('SDL', options : cmake_opt)
	sdl2_dep = sdl2_proj.dependency('SDL2')
endif

if host_machine.system() == 'linux'
  py_dep = dependency('python3-embed')
elif host_machine.system() == 'darwin'
  py_dep = dependency('python3-embed')
  dep_main = dependency('appleframeworks', modules : ['Foundation'])
  dep_libs = dependency('appleframeworks', modules : ['OpenGL'], required : true)
else
  py_dep = dependency('python3')
endif
lua_dep = dependency('lua-5.4')
incdir = include_directories(['lib'])


project_source_files = files([
  # libs
  'lib/ducktape/duktape.c',
  'lib/miniz/miniz.c',

  # tmx lite
	'lib/tmxlite/detail/pugixml.cpp',
	'lib/tmxlite/FreeFuncs.cpp',
	'lib/tmxlite/ImageLayer.cpp',
	'lib/tmxlite/LayerGroup.cpp',
	'lib/tmxlite/Map.cpp',
	'lib/tmxlite/Object.cpp',
	'lib/tmxlite/ObjectGroup.cpp',
	'lib/tmxlite/ObjectTypes.cpp',
	'lib/tmxlite/Property.cpp',
	'lib/tmxlite/TileLayer.cpp',
	'lib/tmxlite/Tileset.cpp',

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
  'src/machines/PythonMachineInterop.cpp',
  'src/machines/PythonMachine.cpp',
  'src/Machine.cpp',

  # graphics
  'src/Renderer.cpp',
  'src/graphics/Rect.cpp',
  'src/graphics/Sprite.cpp',
  'src/graphics/SpriteMap.cpp',
  'src/graphics/SpriteMapGroup.cpp',
  'src/graphics/TextBox.cpp',
  'src/graphics/Texture.cpp',
  'src/graphics/TileMap.cpp',
  'src/graphics/TileMapLayer.cpp',
  'src/graphics/TileMapObject.cpp',
  'src/graphics/Transform.cpp',
  'src/graphics/Transformable.cpp',
  'src/graphics/Vertex.cpp',
  'src/graphics/Viewport.cpp',

  #input
  'src/input/InputManager.cpp',
  'src/input/MouseState.cpp',
  'src/input/GamepadState.cpp',

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


dependencies_py = [
  glew, sdl2_image_dep, sdl2_mixer_dep, ogg_dep, vorbis_dep,
  sdl2_ttf_dep, sdl2_dep, py_dep, lua_dep
]


conf_data = configuration_data()
conf_data.set('version', the_version)


if host_machine.system() == 'darwin'
   dependencies += [dep_main, dep_libs]
   incdir = include_directories(['lib', '/usr/local/include'])
endif


if host_machine.system() == 'windows'
    project_source_files += import('windows').compile_resources( 'resource.rc' )
    incdir = include_directories(['lib', 'C:/Python310/libs'])
endif

# executables without python support.

executable('xentu_debug', project_source_files, include_directories:incdir, 
           dependencies:dependencies, gui_app:false, 
           cpp_args : ['-DXEN_DEBUG','-DXEN_VER="' + the_version + '"'])

executable('xentu', project_source_files, include_directories:incdir,
           dependencies:dependencies, gui_app:true,
           cpp_args : ['-DXEN_VER="' + the_version + '"'])

# executables with python support.

executable('xentu_py_debug', project_source_files, include_directories:incdir, 
           dependencies:dependencies_py, gui_app:false, 
           cpp_args : ['-DXEN_DEBUG', '-DXEN_PY', '-DXEN_VER="' + the_version + '"'])

executable('xentu_py', project_source_files, include_directories:incdir,
           dependencies:dependencies_py, gui_app:true,
           cpp_args : ['-DXEN_PY', '-DXEN_VER="' + the_version + '"'])