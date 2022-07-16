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
			XentuPythonMachine(const int argc, const char *argv[]);
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


	PyObject* PyInit_vfs(void);
	PyObject* PyInit_game(void);
	void PyTriggerFunc(const char* function_name);
}

#endif