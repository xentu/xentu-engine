#include "../../Globals.h"
#include "../../Config.h"
#include "../../Machine.h"
#include "../../vfs/XenVirtualFileSystem.h"
#include "JavaScriptMachine.h"
#include "JavaScriptMachineScripts.h"

namespace xen
{
	JavaScriptMachine::JavaScriptMachine(int argc, char *argv[], Config* config)
	:	Machine::Machine(argc, argv, config)
	{
		// keep a pointer to this instance.
		if (instance != nullptr) {
			XEN_ERROR("> Error, tried to create more than one XentuJavaScriptMachine!");
			exit(111);
		}
		instance = this;

		// load in our custom import loader.
		L = duk_create_heap(NULL, NULL, NULL, NULL, js_error_handler);
		js_init_interop(L);
		for (int i=0; i<xen_js_script_init_c; i++) {
			#if XEN_DEBUG
			duk_peval_string(L, xen_js_script_init[i]);
			#else
			duk_eval_string(L, xen_js_script_init[i]);
			#endif
		}
	}


	JavaScriptMachine* JavaScriptMachine::instance = 0;
	JavaScriptMachine* JavaScriptMachine::GetInstance()
	{
		if (instance == 0)
		{
			XEN_ERROR("Error, called JavaScriptMachine::GetInstance() before initialization!");
			exit(123);
		}
		return instance;
	}


	int JavaScriptMachine::Init()
	{
		// load some js code.
		auto config = this->GetConfig();
		std::string js_code = vfs_get_global()->ReadAllText(config->entry_point) + "\r\ngame.run();\n\n";

		// run the js code.
		duk_eval_string(L, js_code.c_str());

		// finish running.
		return 0;
	}


	int JavaScriptMachine::Trigger(const std::string event_name)
	{
		// todo: not implemented.
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			js_call_func(L, it->second.c_str());
		}
		return 1;
	}


	int JavaScriptMachine::Trigger(const string event_name, const string arg0)
	{
		// todo: not implemented.
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			js_call_func(L, it->second.c_str(), arg0);
		}
		return 1;
	}

	int JavaScriptMachine::Trigger(const string event_name, const int arg0)
	{
		// todo: not implemented.
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			js_call_func(L, it->second.c_str(), arg0);
		}
		return 1;
	}

	int JavaScriptMachine::Trigger(const string event_name, const float arg0)
	{
		// todo: not implemented.
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			js_call_func(L, it->second.c_str(), arg0);
		}
		return 1;
	}


	int JavaScriptMachine::On(const std::string event_name, const std::string callback)
	{
		auto pair = std::make_pair(event_name, callback);
		this->callbacks.insert(pair);
		auto pair2 = std::make_pair(callback, event_name);
		this->callback_aliases.insert(pair2);
		return 1;
	}


	duk_context* JavaScriptMachine::GetContext()
	{
		return L;
	}
	

	const string& JavaScriptMachine::GetEventName(const string& callback)
	{
		auto its = this->callback_aliases.equal_range(callback);
		for (auto it = its.first; it != its.second; ++it) {
			return it->second;
		}
		return callback;
	}

		
	JavaScriptMachine::~JavaScriptMachine()
	{
		duk_destroy_heap(L);
		instance = nullptr;
	}
}