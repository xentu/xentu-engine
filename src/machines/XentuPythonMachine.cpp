#ifndef XEN_PYTHON_MACHINE_CPP
#define XEN_PYTHON_MACHINE_CPP

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <stdio.h>
#include "XentuPythonMachine.h"

#include "src/fs/XenVirtualFileSystem.h"
#include "src/fs/XenFileSystem.h"
#include "src/fs/XenZipFileSystem.h"

namespace xen
{
	XentuPythonMachine::XentuPythonMachine(const int argc, const char *argv[])
	:	XentuMachine::XentuMachine(argc, argv)
	{
		// grab the command line arguments in a format python likes.
		for (int i=0; i<argc && i<MAX_ARGV; i++) {
			arg_values_py[i] = Py_DecodeLocale(argv[i], NULL);
		}

		instance = this;
		
		// get a program name from the args.
		m_program = arg_values_py[0];
		Py_SetProgramName(m_program);

		// load the xen module.
		PyImport_AppendInittab("vfs", &xen_py_init_module_vfs);
		PyImport_AppendInittab("game", &xen_py_init_module_game);

		// initialize python, passing the args.
		Py_Initialize();
		PySys_SetArgv(arg_count, (wchar_t **)arg_values_py);

		// load in our custom import loader.
		PyRun_SimpleString("import sys, importlib.util, vfs\n"
								 "class XenImporter(object):\n"
								 "	def find_module(self, module_name, package_path): return self\n"
								 "	def load_module(self, module_name):\n"
								 "		_source = vfs.read_text_file(\"/\" + module_name.replace(\".py\", \"\") + \".py\")\n"
								 "		if (len(_source)):\n"
								 "			_spec = importlib.util.spec_from_loader(module_name, loader=None)\n"
								 "			_module = importlib.util.module_from_spec(_spec)\n"
								 "			exec(_source, _module.__dict__)\n"
								 "			sys.modules[module_name] = _module\n"
								 "			globals()[module_name] = _module\n"
								 "			return __import__(module_name)\n"
								 "		return self\n"
								 "sys.meta_path.append(XenImporter())\n");

		XEN_LOG("Created XentuPythonMachine\n");
	}


	XentuPythonMachine* XentuPythonMachine::instance = 0;
	XentuPythonMachine* XentuPythonMachine::get_instance()
	{
		if (instance == 0)
		{
			printf("ERROR");
			exit(123);
		}
		return instance;
	}


	int XentuPythonMachine::run(const std::string entry_point)
	{
		XEN_LOG("Python machine started!\n");

		// load some python code.
		std::string py_code = read_text_file(entry_point) + "\r\n";

		// run some python code.
		return PyRun_SimpleString(py_code.c_str());
	}


	int XentuPythonMachine::trigger(const std::string event_name)
	{
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			xen_py_call_func(it->second.c_str());
		}
		return 1;
	}


	int XentuPythonMachine::on(const std::string event_name, const std::string callback_ref)
	{
		auto pair = std::make_pair(event_name, callback_ref);
		this->callbacks.insert(pair);
		return 1;
	}


	XentuPythonMachine::~XentuPythonMachine()
	{
		// tidy up python before closing.
		if (Py_FinalizeEx() < 0) {
			exit(120);
		}
		instance = nullptr;
		PyMem_RawFree(m_program);
		XEN_LOG("Destroyed XentuPythonMachine\n");
	}
}

#endif