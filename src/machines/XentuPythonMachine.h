#pragma once

#include <Python.h>
#include <string>
#include <unordered_map>

#include "../XentuMachine.h"

using namespace std;

namespace xen
{
	class XentuPythonMachine : public XentuMachine
	{
		public:
			XentuPythonMachine(int argc, char *argv[], XentuConfig* config);
			~XentuPythonMachine();
			static XentuPythonMachine* GetInstance();
			int Init();
			int Trigger(const string event_name);
			int On(const string event_name, const string callback_ref);

		private:
			unordered_multimap<string, string> callbacks;
			wchar_t* arg_values_py[12];
			wchar_t* m_program;
			static XentuPythonMachine* instance;
	};


	PyObject* xen_py_init_module_vfs(void);
	PyObject* xen_py_init_module_game(void);
	void xen_py_call_func(const char* function_name);
	void xen_py_call_func(const char* function_name, const char* arg0);
}