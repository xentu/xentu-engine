#ifndef XEN_JS_MACHINE_CPP
#define XEN_JS_MACHINE_CPP

#include <stdio.h>
#include "XentuJavaScriptMachine.h"
#include "../fs/XenVirtualFileSystem.h"

namespace xen
{
	XentuJavaScriptMachine::XentuJavaScriptMachine(const int argc, const char *argv[], const XentuConfig* config)
	:	XentuMachine::XentuMachine(argc, argv, config)
	{
		// keep a pointer to this instance.
		if (jsMachine != nullptr) {
			printf("Error, tried to create more than one XentuJavaScriptMachine!");
			exit(111);
		}
		jsMachine = this;

		L = duk_create_heap(NULL, NULL, NULL, NULL, js_error_handler);

		duk_push_c_function(L, js_native_print, 1 /*nargs*/);
		duk_put_global_string(L, "print");

		XEN_LOG("\nCreated XentuJavaScriptMachine");
	}


	int XentuJavaScriptMachine::init(const std::string entry_point)
	{
		XEN_LOG("Lua machine started!\n");

		// load some js code.
		std::string js_code = vfs_get_global()->ReadAllText(entry_point) + "\r\n";

		// run the js code.
		duk_eval_string(L, js_code.c_str());

		// finish running.
		return 0;
	}


	int XentuJavaScriptMachine::trigger(const std::string event_name)
	{
		// todo: not implemented.
		return 0;
	}
	
		
	XentuJavaScriptMachine::~XentuJavaScriptMachine()
	{
		duk_destroy_heap(L);
		jsMachine = nullptr;
		XEN_LOG("\nDestroyed XentuJavaScriptMachine");
	}
}

#endif