#ifndef XEN_PYTHON_MACHINE
#define XEN_PYTHON_MACHINE

#include <Python.h>
#include <string>
#include <unordered_map>

#include "../XentuMachine.h"

namespace xen
{
	class XentuPythonMachine : public XentuMachine
	{
		public:
			XentuPythonMachine(int argc, char *argv[], const XentuConfig* config);
			~XentuPythonMachine();
			static XentuPythonMachine* get_instance();
			int run(const std::string entry_point);
			int trigger(const std::string event_name);
			int on(const std::string event_name, const std::string callback_ref);

		private:
			std::unordered_multimap<std::string, std::string> callbacks;
			wchar_t* arg_values_py[MAX_ARGV];
			wchar_t* m_program;
			static XentuPythonMachine* instance;
	};


	PyObject* xen_py_init_module_vfs(void);
	PyObject* xen_py_init_module_game(void);
	void xen_py_call_func(const char* function_name);
	void xen_py_call_func(const char* function_name, const char* arg0);
}

#endif