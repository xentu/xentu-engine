#ifndef XEN_PYTHON_MACHINE_CPP
#define XEN_PYTHON_MACHINE_CPP

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <stdio.h>
#include "XentuPythonMachine.h"
#include "XentuPythonMachineScripts.h"
#include "../fs/XenVirtualFileSystem.h"

namespace xen
{
	XentuPythonMachine::XentuPythonMachine(int argc, char *argv[], XentuConfig* config)
	:	XentuMachine::XentuMachine(argc, argv, config)
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
		PyRun_SimpleString(xen_py_script_init);

		XEN_LOG("- Created XentuPythonMachine\n");
	}


	XentuPythonMachine* XentuPythonMachine::instance = 0;
	XentuPythonMachine* XentuPythonMachine::GetInstance()
	{
		if (instance == 0)
		{
			printf("ERROR");
			exit(123);
		}
		return instance;
	}


	int XentuPythonMachine::Init()
	{
		XEN_LOG("- Python machine started!\n");

		// load some python code.
		auto config = this->GetConfig();
		std::string py_code = vfs_get_global()->ReadAllText(config->entry_point) + "\r\n";

		// run some python code.
		return PyRun_SimpleString(py_code.c_str());
	}


	int XentuPythonMachine::Trigger(const std::string event_name)
	{
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			xen_py_call_func(it->second.c_str());
		}
		return 1;
	}


	int XentuPythonMachine::On(const std::string event_name, const std::string callback_ref)
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
		XEN_LOG("- Destroyed XentuPythonMachine\n");
	}
}

#endif