#ifndef XEN_JS_MACHINE_INTEROP_CPP
#define XEN_JS_MACHINE_INTEROP_CPP

#include <stdio.h>
#include "XentuJavaScriptMachine.h"

namespace xen
{
	duk_ret_t js_native_print(duk_context *ctx) {
		printf("%s\n", duk_to_string(ctx, 0));
		return 0;  /* no return value (= undefined) */
	}


	void js_error_handler(void *udata, const char *msg) {
		(void) udata;  /* ignored in this case, silence warning */

		/* Note that 'msg' may be NULL. */
		fprintf(stderr, "*** FATAL ERROR: %s\n", (msg ? msg : "no message"));
		fflush(stderr);
		abort();
	}
}

#endif