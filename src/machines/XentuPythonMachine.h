#ifndef XEN_PYTHON_MACHINE
#define XEN_PYTHON_MACHINE

#include "../XentuMachine.h"

namespace xen
{
	class XentuPythonMachine : public XentuMachine
	{
		public:
				XentuPythonMachine();
				~XentuPythonMachine();
				int run();
				void set_global(const std::string name, const std::string value);
	};
}

#endif