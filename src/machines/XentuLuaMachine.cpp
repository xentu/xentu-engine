#ifndef XEN_LUA_MACHINE_CPP
#define XEN_LUA_MACHINE_CPP

#include <stdio.h>
#include "XentuLuaMachine.h"

namespace xen
{
	XentuLuaMachine::XentuLuaMachine()
	{
		printf("\nCreated XentuLuaMachine");
	}

	void XentuLuaMachine::set_global(const std::string name, const std::string value)
	{
		printf("\nLua Set [%s]: %s", name.c_str(), value.c_str());
	}
		
	XentuLuaMachine::~XentuLuaMachine()
	{
		printf("\nDestroyed XentuLuaMachine");
	}
}

#endif