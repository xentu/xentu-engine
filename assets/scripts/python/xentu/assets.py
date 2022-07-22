# Module used for communicating with the virtual file system.


def mount(virtual_path, real_path):
	"""
   Mount a location into the virtual file system to load assets from.
   Arguments:
      virtual_path: The virtual path where assets will be loadable from
      real_path: The real path where the assets lay (can be a zip file!)
   """
	pass


def load_texture(filename) -> int:
	"""
	Load a texture from the virtual file system.
	Arguments:
		filename: The virtual location of the texture file to load.
	Returns:
		An integer reference for the loaded texture.
	"""
	return -1


def read_text_file(filename) -> str:
	"""
	Load a text file from the virtual file system.
	Arguments:
		filename: The virtual location of the text file to load.
	Returns:
		The text loaded from the file.
	"""
	pass