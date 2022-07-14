#ifndef XEN_JS_MACHINE_CPP
#define XEN_JS_MACHINE_CPP

#include <stdio.h>
#include "XentuJavaScriptMachine.h"

namespace xen
{
	// ------------------ native javascript methods  ---------------------------


	static duk_ret_t js_native_print(duk_context *ctx) {
		printf("%s\n", duk_to_string(ctx, 0));
		return 0;  /* no return value (= undefined) */
	}


	static void js_error_handler(void *udata, const char *msg) {
		(void) udata;  /* ignored in this case, silence warning */

		/* Note that 'msg' may be NULL. */
		fprintf(stderr, "*** FATAL ERROR: %s\n", (msg ? msg : "no message"));
		fflush(stderr);
		abort();
	}


	// ------------------ native javascript methods  ---------------------------


	XentuJavaScriptMachine::XentuJavaScriptMachine(const int argc, const char *argv[])
	:	XentuMachine::XentuMachine(argc, argv)
	{
		XEN_LOG("\nCreated XentuJavaScriptMachine");
		L = duk_create_heap(NULL, NULL, NULL, NULL, js_error_handler);

		duk_push_c_function(L, js_native_print, 1 /*nargs*/);
		duk_put_global_string(L, "print");
	}


	int XentuJavaScriptMachine::run(const std::string entry_point)
	{
		XEN_LOG("Lua machine started!\n");

		// load some js code.
		std::string js_code = read_text_file("/test.js") + "\r\n";

		// run the js code.
		duk_eval_string(L, js_code.c_str());

		// finish running.
		return 0;
	}
	
		
	XentuJavaScriptMachine::~XentuJavaScriptMachine()
	{
		duk_destroy_heap(L);
		XEN_LOG("\nDestroyed XentuJavaScriptMachine");
	}
}

#endif