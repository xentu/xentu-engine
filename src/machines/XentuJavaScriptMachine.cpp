#ifndef XEN_JS_MACHINE_CPP
#define XEN_JS_MACHINE_CPP

#include <stdio.h>
#include "XentuJavaScriptMachine.h"
#include "XentuJavaScriptMachineScripts.h"
#include "../fs/XenVirtualFileSystem.h"

namespace xen
{
	XentuJavaScriptMachine::XentuJavaScriptMachine(int argc, char *argv[], XentuConfig* config)
	:	XentuMachine::XentuMachine(argc, argv, config)
	{
		XEN_LOG("- Created XentuJavaScriptMachine.\n");

		// keep a pointer to this instance.
		if (instance != nullptr) {
			printf("Error, tried to create more than one XentuJavaScriptMachine!");
			exit(111);
		}
		instance = this;

		// load in our custom import loader.

		L = duk_create_heap(NULL, NULL, NULL, NULL, js_error_handler);
		js_init_interop(L);
		duk_eval_string(L, xen_js_script_init);
	}


	XentuJavaScriptMachine* XentuJavaScriptMachine::instance = 0;
	XentuJavaScriptMachine* XentuJavaScriptMachine::GetInstance()
	{
		if (instance == 0)
		{
			printf("ERROR");
			exit(123);
		}
		return instance;
	}


	int XentuJavaScriptMachine::Init()
	{
		XEN_LOG("- Started JavaScriptMachine.\n");

		// load some js code.
		auto config = this->GetConfig();
		std::string js_code = vfs_get_global()->ReadAllText(config->entry_point) + "\r\n";

		// run the js code.
		duk_eval_string(L, js_code.c_str());

		// finish running.
		return 0;
	}


	int XentuJavaScriptMachine::Trigger(const std::string event_name)
	{
		// todo: not implemented.
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			js_call_func(L, it->second.c_str());
		}
		return 1;
	}


	int XentuJavaScriptMachine::On(const std::string event_name, const std::string callback)
	{
		auto pair = std::make_pair(event_name, callback);
		this->callbacks.insert(pair);
		return 1;
	}
	
		
	XentuJavaScriptMachine::~XentuJavaScriptMachine()
	{
		duk_destroy_heap(L);
		instance = nullptr;
		XEN_LOG("- Destroyed XentuJavaScriptMachine.\n");
	}
}

#endif