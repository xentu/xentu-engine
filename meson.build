project('xentu_rebuild', 'c', 'cpp',
        default_options: ['default_library=static'])

#sdl2_image_dep = dependency('sdl2_image')
#sdl2_dep = dependency('sdl2')
py_dep = dependency('python3')
lua_dep = dependency('lua-5.4')
incdir = include_directories(['lib'])


project_source_files = files([
  # libs
  'lib/ducktape/duktape.c',
  'lib/miniz/miniz.c',

  # filesystem
  'src/fs/XenFileInfo.cpp',
  'src/fs/XenMemoryFile.cpp',
  'src/fs/XenMemoryFileSystem.cpp',
  'src/fs/XenNativeFile.cpp',
  'src/fs/XenNativeFileSystem.cpp',
  'src/fs/XenStringUtils.cpp',
  'src/fs/XenVirtualFileSystem.cpp',
  'src/fs/XenZipFile.cpp',
  'src/fs/XenZipFileSystem.cpp',

  # machines
  #'src/machines/XentuJavaScriptMachineInterop.cpp',
  #'src/machines/XentuJavaScriptMachine.cpp',
  'src/machines/XentuLuaMachine.cpp',
  'src/machines/XentuPythonMachineInterop.cpp',
  'src/machines/XentuPythonMachine.cpp',
  'src/XentuMachine.cpp', 

  # main
  'main.cpp'
])


executable('xentu', project_source_files,
           include_directories : incdir,
           dependencies: [py_dep, lua_dep])