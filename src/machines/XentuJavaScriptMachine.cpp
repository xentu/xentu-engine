#ifndef XEN_JS_MACHINE_CPP
#define XEN_JS_MACHINE_CPP

#include <stdio.h>
#include "XentuJavaScriptMachine.h"
#include "../fs/XenVirtualFileSystem.h"

namespace xen
{
	XentuJavaScriptMachine::XentuJavaScriptMachine(int argc, char *argv[], const XentuConfig* config)
	:	XentuMachine::XentuMachine(argc, argv, config)
	{
		XEN_LOG("Constructor for XentuJavaScriptMachine called.\n");

		// keep a pointer to this instance.
		if (instance != nullptr) {
			printf("Error, tried to create more than one XentuJavaScriptMachine!");
			exit(111);
		}
		instance = this;

		L = duk_create_heap(NULL, NULL, NULL, NULL, js_error_handler);
		js_init_interop(L);
	}


	XentuJavaScriptMachine* XentuJavaScriptMachine::instance = 0;
	XentuJavaScriptMachine* XentuJavaScriptMachine::get_instance()
	{
		if (instance == 0)
		{
			printf("ERROR");
			exit(123);
		}
		return instance;
	}


	int XentuJavaScriptMachine::init(const std::string entry_point)
	{
		XEN_LOG("JavaScript machine started!\n");

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
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			js_call_func(L, it->second.c_str());
		}
		return 1;
	}


	int XentuJavaScriptMachine::on(const std::string event_name, const std::string callback)
	{
		auto pair = std::make_pair(event_name, callback);
		this->callbacks.insert(pair);
		return 1;
	}
	
		
	XentuJavaScriptMachine::~XentuJavaScriptMachine()
	{
		duk_destroy_heap(L);
		instance = nullptr;
		XEN_LOG("Destroyed XentuJavaScriptMachine.\n");
	}
}

#endif