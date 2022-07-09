#ifndef XEN_PYTHON_MACHINE_CPP
#define XEN_PYTHON_MACHINE_CPP

#include <stdio.h>
#include "XentuPythonMachine.h"

namespace xen
{
	XentuPythonMachine::XentuPythonMachine()
	{
		printf("\nCreated XentuPythonMachine");
	}

	int XentuPythonMachine::run()
	{
		printf("\nPython machine started!");

		// todo: game loop goes here!

		return 0;
	}

	void XentuPythonMachine::set_global(const std::string name, const std::string value)
	{
		printf("\nPython Set [%s]: %s", name.c_str(), value.c_str());
	}
		
	XentuPythonMachine::~XentuPythonMachine()
	{
		printf("\nDestroyed XentuPythonMachine");
	}
}

#endif