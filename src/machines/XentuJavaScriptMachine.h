#ifndef XEN_JS_MACHINE
#define XEN_JS_MACHINE

#include "../XentuMachine.h"

#include <ducktape/duktape.h>

namespace xen
{
	class XentuJavaScriptMachine : public XentuMachine
	{
		public:
			XentuJavaScriptMachine(const int argc, const char *argv[], const XentuConfig* config);
			~XentuJavaScriptMachine();
			int init(const std::string entry_point);
			int trigger(const std::string event_name);
		
		private:
			duk_context* L;
	};


	// when using the js machine, this will be used to bridge communication.
	static XentuJavaScriptMachine* jsMachine = nullptr;


	duk_ret_t js_native_print(duk_context *ctx);
	void js_error_handler(void *udata, const char *msg);
}

#endif