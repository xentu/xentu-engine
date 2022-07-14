#ifndef XEN_JS_MACHINE
#define XEN_JS_MACHINE

#include "../XentuMachine.h"

#include <ducktape/duktape.h>

namespace xen
{
	class XentuJavaScriptMachine : public XentuMachine
	{
		public:
			XentuJavaScriptMachine(const int argc, const char *argv[]);
			~XentuJavaScriptMachine();
			int run(const std::string entry_point);
		
		private:
			duk_context* L;
	};
}

#endif