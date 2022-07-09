project('xentu_rebuild', 'c', 'cpp',
        default_options: ['default_library=static'])


#sdl2_image_dep = dependency('sdl2_image')
#sdl2_dep = dependency('sdl2')
incdir = include_directories('lib')


project_source_files = files([
  'src/fs/miniz.c',
  'src/fs/XenFileInfo.cpp',
  'src/fs/XenMemoryFile.cpp',
  'src/fs/XenMemoryFileSystem.cpp',
  'src/fs/XenNativeFile.cpp',
  'src/fs/XenNativeFileSystem.cpp',
  'src/fs/XenStringUtils.cpp',
  'src/fs/XenVirtualFileSystem.cpp',
  'src/fs/XenZipFile.cpp',
  'src/fs/XenZipFileSystem.cpp',
  'src/machines/XentuLuaMachine.cpp',
  'src/machines/XentuPythonMachine.cpp',
  'src/XentuMachine.cpp', 
  'main.cpp'
])


executable('xentu', project_source_files,
           include_directories : incdir,
           dependencies: [])
#          dependencies: [sdl2_image_dep, sdl2_dep],