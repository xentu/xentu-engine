#ifndef XEN_PYTHON_MACHINE_CPP
#define XEN_PYTHON_MACHINE_CPP

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <stdio.h>
#include "XentuPythonMachine.h"

#include "src/fs/XenVirtualFileSystem.h"
#include "src/fs/XenFileSystem.h"
#include "src/fs/XenZipFileSystem.h"

namespace xen
{
	// when using the python machine, this will be used to bridge communication
	// between the class instance python.
	static XentuPythonMachine* pyMachine = nullptr;



	// ------------------ vfs python module ------------------------------------



	/* Load a text file from the vfs. */
	static PyObject* vfs_py_read_text_file(PyObject *self, PyObject *args) {
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

	/* Mount a zip archive to the vfs. */
	static PyObject* vfs_py_mount(PyObject *self, PyObject *args) {
		char *s_point;
		char *s_path;
		if (!PyArg_ParseTuple(args, "ss", &s_point, &s_path)) {
			return NULL;
		}

		// create the file system mount & init.
		XenFileSystemPtr zip_fs(new XenZipFileSystem(s_path, "/"));
		zip_fs->Initialize();
		
		// add the file systems to the vfs.
		XenVirtualFileSystemPtr vfs = vfs_get_global();
    	vfs->AddFileSystem(s_point, zip_fs);

		// return true
		return PyBool_FromLong(1);
	}

	static PyMethodDef VfsPyMethods[] = {
		{"read_text_file", vfs_py_read_text_file, METH_VARARGS, "Use the xentu vfs to get file."},
		{"mount", vfs_py_mount, METH_VARARGS, "Mount a path or zip archive into the vfs."},
		{NULL, NULL, 0, NULL}
	};

	static PyModuleDef VfsPyModule = {
		PyModuleDef_HEAD_INIT, "vfs", NULL, -1, VfsPyMethods,
		NULL, NULL, NULL, NULL
	};

	static PyObject* PyInit_xen_pi(void) {
		return PyModule_Create(&VfsPyModule);
	}



	// ------------------ xen python module ------------------------------------



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
		PyImport_AppendInittab("vfs", &PyInit_xen_pi);

		// initialize python, passing the args.
		Py_Initialize();
		PySys_SetArgv(arg_count, (wchar_t **)arg_values_py);

		// load in our custom import loader.
		PyRun_SimpleString("import sys, importlib.util, vfs\n"
								 "class XenImporter(object):\n"
								 "	def find_module(self, module_name, package_path): return self\n"
								 "	def load_module(self, module_name):\n"
								 "		_source = vfs.read_text_file(\"/\" + module_name.replace(\".py\", \"\") + \".py\")\n"
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
		std::string py_code = read_text_file("/game.py") + "\r\n";

		// run some python code.
		return PyRun_SimpleString(py_code.c_str());
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