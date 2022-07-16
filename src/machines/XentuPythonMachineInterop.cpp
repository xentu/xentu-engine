#ifndef XEN_PYTHON_MACHINE_INTEROP_CPP
#define XEN_PYTHON_MACHINE_INTEROP_CPP

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <stdio.h>
#include <string>
#include "XentuPythonMachine.h"

#include "src/fs/XenVirtualFileSystem.h"
#include "src/fs/XenFileSystem.h"
#include "src/fs/XenZipFileSystem.h"

namespace xen
{
	/* Load a text file from the vfs. */
	PyObject* py_vfs_read_text_file(PyObject *self, PyObject *args) {
		char *s;
		if (!PyArg_ParseTuple(args, "s", &s)) {
			return NULL;
		}

		auto m = XentuPythonMachine::get_instance();
		
		std::string result = m->read_text_file(s);;
		return PyUnicode_FromString(result.data());
	}


	/* Mount a zip archive to the vfs. */
	PyObject* py_vfs_mount(PyObject *self, PyObject *args) {
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


	/* handle a game event */
	PyObject* py_game_on(PyObject *self, PyObject *args) {
		char *s_event_name;
		char *s_callback;
		if (!PyArg_ParseTuple(args, "ss", &s_event_name, &s_callback)) {
			return NULL;
		}

		printf("game.on was called passing the event named %s -> %s\n", s_event_name, s_callback);

		auto m = XentuPythonMachine::get_instance();
		m->on(s_event_name, s_callback);

		// return true
		return PyBool_FromLong(1);
	}


	/* call a named python function */
	void PyTriggerFunc(const char* function_name)
	{
		PyObject *pModule = PyImport_ImportModule("__main__");
		PyObject *pFunc = PyObject_GetAttrString(pModule, function_name);
		if (pFunc == NULL) {
			if (PyErr_Occurred()) {
				PyErr_Print();
			}
			return;
		}

		PyObject_CallObject(pFunc, NULL);
	}


	/* handle a game event */
	PyObject* py_game_trigger(PyObject *self, PyObject *args) {
		char *s_event_name;
		if (!PyArg_ParseTuple(args, "s", &s_event_name)) {
			return NULL;
		}

		// trigger the event.
		auto m = XentuPythonMachine::get_instance();
		m->trigger(s_event_name);

		// return true
		return PyBool_FromLong(1);
	}


	PyMethodDef PyVfsMethods[] = {
		{"read_text_file", py_vfs_read_text_file, METH_VARARGS, "Use the xentu vfs to get file."},
		{"mount", py_vfs_mount, METH_VARARGS, "Mount a path or zip archive into the vfs."},
		{NULL, NULL, 0, NULL}
	};


	PyMethodDef PyGameMethods[] = {
		{"on", py_game_on, METH_VARARGS, "Handle an engine or custom event."},
		{"trigger", py_game_trigger, METH_VARARGS, "Trigger an event."},
		{NULL, NULL, 0, NULL}
	};


	PyModuleDef PyVfsModule = {
		PyModuleDef_HEAD_INIT, "vfs", NULL, -1, PyVfsMethods,
		NULL, NULL, NULL, NULL
	};


	PyModuleDef PyGameModule = {
		PyModuleDef_HEAD_INIT, "game", NULL, -1, PyGameMethods,
		NULL, NULL, NULL, NULL
	};


	PyObject* PyInit_vfs(void) {
		return PyModule_Create(&PyVfsModule);
	}


	PyObject* PyInit_game(void) {
		return PyModule_Create(&PyGameModule);
	}
}

#endif