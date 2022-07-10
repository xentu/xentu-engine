#ifndef XEN_PYTHON_MACHINE_CPP
#define XEN_PYTHON_MACHINE_CPP

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdio.h>
#include "XentuPythonMachine.h"

namespace xen
{
	XentuPythonMachine::XentuPythonMachine(const int argc, const char *argv[])
	:	XentuMachine::XentuMachine(argc, argv)
	{
		// grab program name from the command line arguments.
		m_program = Py_DecodeLocale(argv[0], NULL);
		if (m_program == NULL) {
			fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
			exit(1);
		}
		Py_SetProgramName(m_program);

		// initialize python, passing the command line arguments.
		Py_Initialize();
		PySys_SetArgv(arg_count, (wchar_t **)argv);

		XEN_LOG("\nCreated XentuPythonMachine");
	}


	int XentuPythonMachine::run()
	{
		XEN_LOG("\nPython machine started!\n");

		// run some python code.
		PyRun_SimpleString("print('\\nHello from python world!')\n");

		return 0;
	}


	void XentuPythonMachine::set_global(const std::string name, const std::string value)
	{
		printf("\nPython Set [%s]: %s", name.c_str(), value.c_str());
	}


	XentuPythonMachine::~XentuPythonMachine()
	{
		// tidy up python before closing.
		if (Py_FinalizeEx() < 0) {
			exit(120);
		}
		PyMem_RawFree(m_program);
		XEN_LOG("\nDestroyed XentuPythonMachine");
	}
}

#endif