#ifndef XEN_LUA_MACHINE_CPP
#define XEN_LUA_MACHINE_CPP

#include <stdio.h>
#include "XentuLuaMachine.h"

namespace xen
{
	XentuLuaMachine::XentuLuaMachine(const int argc, const char *argv[])
	:	XentuMachine::XentuMachine(argc, argv)
	{
		XEN_LOG("\nCreated XentuLuaMachine");
	}
	
		
	XentuLuaMachine::~XentuLuaMachine()
	{
		XEN_LOG("\nDestroyed XentuLuaMachine");
	}
}

#endif