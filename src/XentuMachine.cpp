#ifndef XEN_ENVIRONMENT_CPP
#define XEN_ENVIRONMENT_CPP

#include <stdio.h>
#include <string>
#include "XentuMachine.h"

namespace xen
{
	XentuMachine::XentuMachine()
	{
		printf("\nCreated XentuMachine");
				
	}

	int XentuMachine::run()
	{
		// todo: throw error if trying to use the default machine.
		return -1;
	}

	void XentuMachine::set_global(const std::string name, const std::string value)
	{
		// todo: throw error if trying to use the default machine.
		//printf("\nSet [%s]: %s", name.c_str(), value.c_str());
	}
		
	XentuMachine::~XentuMachine()
	{
		printf("\nDestroyed XentuMachine");
	}
}

#endif