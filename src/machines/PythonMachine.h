#if XEN_PY
#pragma once

#ifdef _DEBUG
  #undef _DEBUG
  #include <Python.h>
  #define _DEBUG
#else
  #include <Python.h>
#endif

#include <string>
#include <unordered_map>

#include "../Machine.h"

using namespace std;

namespace xen
{
	class PythonMachine : public Machine
	{
		public:
			PythonMachine(int argc, char *argv[], Config* config);
			~PythonMachine();
			static PythonMachine* GetInstance();
			int Init();
			int Trigger(const string event_name);
			int Trigger(const string event_name, const string arg0);
			int Trigger(const string event_name, const int arg0);
			int Trigger(const string event_name, const float arg0);
			int On(const string event_name, const string callback_ref);

		private:
			unordered_multimap<string, string> callbacks;
			wchar_t* arg_values_py[12];
			wchar_t* m_program;
			static PythonMachine* instance;
	};


	PyObject* xen_py_init_interop(void);
	void xen_py_call_func(const char* function_name);
	void xen_py_call_func(const char* function_name, const string arg0);
	void xen_py_call_func(const char* function_name, const int arg0);
	void xen_py_call_func(const char* function_name, const float arg0);
}

#endif