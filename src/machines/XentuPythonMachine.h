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
			int run(const std::string entry_point);
		
		private:
			wchar_t* arg_values_py[MAX_ARGV];
			wchar_t* m_program;
	};
}

#endif