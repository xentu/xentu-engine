#ifndef XEN_PYTHON_MACHINE_SCRIPTS_H
#define XEN_PYTHON_MACHINE_SCRIPTS_H
/**
 * This file holds the python code to be executed when a python engine is loaded
 * by the user. The file does the following:
 * 
 * - Inject an extra loader into importlib that uses the engines vfs.
 * - Add py exclusive modules (const, geometry) under the xentu dummy module.
 * - Add aliases for c++ modules like vfs under the xentu dummy module.
 */
namespace xen { const char * xen_py_script_init = R"(



import sys, importlib.util, game, vfs
from types import ModuleType



class XenImporter(object):
	def find_module(self, module_name, package_path): return self
	def load_module(self, module_name):
		_source = vfs.read_text_file("/" + module_name.replace(".py", "") + ".py")
		if (len(_source)):
			_spec = importlib.util.spec_from_loader(module_name, loader=None)
			_module = importlib.util.module_from_spec(_spec)
			exec(_source, _module.__dict__)
			sys.modules[module_name] = _module
			globals()[module_name] = _module
			return __import__(module_name)
		return self
sys.meta_path.append(XenImporter())



class m_xentu(ModuleType):
	class const(ModuleType):
		# coordinate constants.
		X_AUTO = -1 # window x coord auto
		Y_AUTO = -1 # window y coord auto
		# renderer mode
		RM_SOFTWARE = 0 # software rendering
		RM_ACCELERATED = 1 # hardware accelerated rendering
		# rendering technique
		RT_IMMEDIATE = 0 # forward rendering, draw things immediately
		RT_DEFERRED = 1 # deferred rendering using begin() and end()

	class geometry(ModuleType):
		class Rect:
			def __init__(self, x:int, y:int, width:int, height:int):
				self.x = x
				self.y = y
				self.width = width
				self.height = height
		class Sprite:
			def __init__(self, texture:int, rect):
				self.texture = texture
				self.rect = rect



globals()['xentu'] = sys.modules['xentu'] = m_xentu('xentu')
globals()['xentu.assets'] = sys.modules['xentu.assets'] = vfs
globals()['xentu.game'] = sys.modules['xentu.game'] = game



)"; }
#endif