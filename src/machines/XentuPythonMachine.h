#ifndef XEN_PYTHON_MACHINE
#define XEN_PYTHON_MACHINE

#include "../XentuMachine.h"

namespace xen
{
	class XentuPythonMachine : public XentuMachine
	{
		public:
			XentuPythonMachine(const int argc, const char *argv[]);
			~XentuPythonMachine();
			int run();
			void set_global(const std::string name, const std::string value);
		
		protected:
			wchar_t *m_program;
	};
}

#endif