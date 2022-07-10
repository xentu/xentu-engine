#ifndef XEN_PYTHON_MACHINE_CPP
#define XEN_PYTHON_MACHINE_CPP

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdio.h>
#include "XentuPythonMachine.h"

namespace xen
{
	// when using the python machine, this will be used to bridge communication
	// between the class instance python.
	static XentuPythonMachine* pyMachine = nullptr;

	/* Return the number of arguments of the application command line */
	static PyObject* xen_py_read_text_file(PyObject *self, PyObject *args) {
		char *s;
		if (!PyArg_ParseTuple(args, "s", &s)) {
			return NULL;
		}

		if (pyMachine == nullptr) {
			printf("Error, pyMachine is not initialized.");
			return NULL;
		}

		std::string result = pyMachine->read_text_file(s);
		return PyUnicode_FromString(result.data());
	}

	static PyMethodDef XenPyMethods[] = {
		{"read_text_file", xen_py_read_text_file, METH_VARARGS, "Use the xentu vfs to get file."},
		{NULL, NULL, 0, NULL}
	};

	static PyModuleDef XenPyModule = {
		PyModuleDef_HEAD_INIT, "xen", NULL, -1, XenPyMethods,
		NULL, NULL, NULL, NULL
	};

	static PyObject* PyInit_xen_pi(void) {
		return PyModule_Create(&XenPyModule);
	}



	XentuPythonMachine::XentuPythonMachine(const int argc, const char *argv[])
	:	XentuMachine::XentuMachine(argc, argv)
	{
		// keep a pointer to this instance.
		if (pyMachine != nullptr) {
			printf("Error, tried to create more than one XentuPythonMachine!");
			exit(111);
		}
		pyMachine = this;

		// grab the command line arguments in a format python likes.
		for (int i=0; i<argc && i<MAX_ARGV; i++) {
			arg_values_py[i] = Py_DecodeLocale(argv[i], NULL);
		}
		
		// get a program name from the args.
		m_program = arg_values_py[0];
		Py_SetProgramName(m_program);

		// load the xen module.
		PyImport_AppendInittab("xen", &PyInit_xen_pi);

		// initialize python, passing the args.
		Py_Initialize();
		PySys_SetArgv(arg_count, (wchar_t **)arg_values_py);

		// load in our custom import loader.
		PyRun_SimpleString("import sys, importlib.util, xen\n"
								 "class XenImporter(object):\n"
								 "	def find_module(self, module_name, package_path): return self\n"
								 "	def load_module(self, module_name):\n"
								 "		_source = xen.read_text_file(\"/\" + module_name.replace(\".py\", \"\") + \".py\")\n"
								 "		if (len(_source)):\n"
								 "			_spec = importlib.util.spec_from_loader(module_name, loader=None)\n"
								 "			_module = importlib.util.module_from_spec(_spec)\n"
								 "			exec(_source, _module.__dict__)\n"
								 "			sys.modules[module_name] = _module\n"
								 "			globals()[module_name] = _module\n"
								 "			return __import__(module_name)\n"
								 "		return self\n"
								 "sys.meta_path.append(XenImporter())\n");

		XEN_LOG("Created XentuPythonMachine\n");
	}


	int XentuPythonMachine::run()
	{
		XEN_LOG("Python machine started!\n");

		// load some python code.
		std::string py_code = read_text_file("/test.py") + "\r\n";

		// run some python code.
		PyRun_SimpleString(py_code.c_str());

		return 0;
	}


	XentuPythonMachine::~XentuPythonMachine()
	{
		// tidy up python before closing.
		if (Py_FinalizeEx() < 0) {
			exit(120);
		}
		PyMem_RawFree(m_program);
		pyMachine = nullptr;
		XEN_LOG("Destroyed XentuPythonMachine\n");
	}
}

#endif