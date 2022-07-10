import sys, importlib.util, time
import xen


# --- custom loader ---------------------------------------------------------- #
def xen_import(name):
	_path = "/" + name.replace(".py", "") + ".py"
	_source = xen.read_text_file(_path)
	_spec = importlib.util.spec_from_loader(name, loader=None)
	_module = importlib.util.module_from_spec(_spec)
	exec(_source, _module.__dict__)
	sys.modules[name] = _module
	globals()[name] = _module
	return _module
# --- test code  ------------------------------------------------------------- #

module1 = xen_import("module1")
module1.test_module('this works!')

time.sleep(3) # Sleep for 3 seconds
print('slept for 3s')
print("done.")