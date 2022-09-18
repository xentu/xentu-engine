#define PY_SSIZE_T_CLEAN

#include "SDL.h"

#include <Python.h>
#include <stdio.h>
#include <string>
#include "XentuPythonMachine.h"

#include "src/fs/XenVirtualFileSystem.h"
#include "src/fs/XenFileSystem.h"
#include "src/fs/XenZipFileSystem.h"

namespace xen
{
	/* ---- Python Interop Helpers ------------------------------------------- */


	void xen_py_call_func(const char* function_name)
	{
		PyObject *pModule = PyImport_ImportModule("__main__");
		PyObject *pFunc = PyObject_GetAttrString(pModule, function_name);
		if (pFunc == NULL) {
			if (PyErr_Occurred()) {
				PyErr_Print();
			}
			Py_DECREF(pModule);
			return;
		}
		PyObject_CallNoArgs(pFunc);
		Py_DECREF(pFunc);
		Py_DECREF(pModule);
	}


	void xen_py_call_func(const char* function_name, const char* arg0)
	{
		PyObject *pModule = PyImport_ImportModule("__main__");
		PyObject *pFunc = PyObject_GetAttrString(pModule, function_name);
		if (pFunc == NULL) {
			if (PyErr_Occurred()) {
				PyErr_Print();
			}
			Py_DECREF(pModule);
			return;
		}
		PyObject *pArg0 = PyUnicode_FromString(arg0);
		PyObject_CallOneArg(pFunc, pArg0);
		Py_DECREF(pArg0);
		Py_DECREF(pFunc);
		Py_DECREF(pModule);
	}


	/* ---- VFS Module ------------------------------------------------------- */

	
	PyObject* xen_py_fn_vfs_read_text_file(PyObject *self, PyObject *args) {
		char *s;
		if (!PyArg_ParseTuple(args, "s", &s)) {
			return NULL;
		}

		std::string result = vfs_get_global()->ReadAllText(s);
		return PyUnicode_FromString(result.data());
	}


	PyObject* xen_py_fn_vfs_load_texture(PyObject *self, PyObject *args) {
		char *s;
		if (!PyArg_ParseTuple(args, "s", &s)) {
			return NULL;
		}

		auto m = XentuPythonMachine::GetInstance();
		auto r = m->GetRenderer();

		xen::VfsBufferResult res = vfs_get_global()->ReadAllData(s);
		printf("Bytes read: %s\n", std::to_string(res.length).c_str());
		int texture_id = r->LoadTexture(res.buffer, res.length);

		// todo: take the filename from s and load the texture.
		return PyLong_FromLong(texture_id);
	}


	PyObject* xen_py_fn_vfs_mount(PyObject *self, PyObject *args) {
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


	PyMethodDef xen_py_explain_module_vfs[] = {
		{"read_text_file", xen_py_fn_vfs_read_text_file, METH_VARARGS, "Use the xentu vfs to get file."},
		{"load_texture", xen_py_fn_vfs_load_texture, METH_VARARGS, "Use the xentu vfs to load a texture."},
		{"mount", xen_py_fn_vfs_mount, METH_VARARGS, "Mount a path or zip archive into the vfs."},
		{NULL, NULL, 0, NULL}
	};


	PyModuleDef xen_py_def_module_vfs = {
		PyModuleDef_HEAD_INIT, "vfs", NULL, -1, xen_py_explain_module_vfs,
		NULL, NULL, NULL, NULL
	};


	PyObject* xen_py_init_module_vfs(void) {
		return PyModule_Create(&xen_py_def_module_vfs);
	}


	/* ---- Game Module ------------------------------------------------------ */


	PyObject* xen_py_fn_game_on(PyObject *self, PyObject *args) {
		char *s_event_name;
		char *s_callback;
		if (!PyArg_ParseTuple(args, "ss", &s_event_name, &s_callback)) {
			return NULL;
		}

		printf("game.on was called passing the event named %s -> %s\n", s_event_name, s_callback);

		auto m = XentuPythonMachine::GetInstance();
		m->On(s_event_name, s_callback);

		// return true
		return PyBool_FromLong(1);
	}


	PyObject* xen_py_fn_game_trigger(PyObject *self, PyObject *args) {
		char *s_event_name;
		if (!PyArg_ParseTuple(args, "s", &s_event_name)) {
			return NULL;
		}

		// trigger the event.
		auto m = XentuPythonMachine::GetInstance();
		m->Trigger(s_event_name);

		// return true
		return PyBool_FromLong(1);
	}


	PyObject* xen_py_fn_game_create_window(PyObject *self, PyObject *args) {
		XentuPythonMachine* m = XentuPythonMachine::GetInstance();
		auto r = m->GetRenderer();
		int window_id = r->CreateWindow();
		return PyLong_FromLong(window_id);
	}


	PyObject* xen_py_fn_game_create_window_ex(PyObject *self, PyObject *args) {
		char *s_title;
		int s_x, s_y, s_width, s_height, s_mode;
		if (!PyArg_ParseTuple(args, "siiiii", &s_title, &s_x, &s_y, &s_width, &s_height, &s_mode)) {
			return NULL;
		}

		XentuPythonMachine* m = XentuPythonMachine::GetInstance();
		auto r = m->GetRenderer();
		auto t = std::string(s_title);
		int window_id = r->CreateWindowEx(t, s_x, s_y, s_width, s_height, s_mode);
		return PyLong_FromLong(window_id);
	}


	PyObject* xen_py_fn_game_run(PyObject *self, PyObject *args) {
		XentuPythonMachine* m = XentuPythonMachine::GetInstance();
		//auto r = m->GetRenderer();
		//r->run();
		m->Run();
		return PyBool_FromLong(1);
	}


	int xen_py_read_attr_int(PyObject* obj, char* name) {
		if (!PyObject_HasAttrString(obj, name)) return NULL;
		PyObject* a_result = PyObject_GetAttrString(obj, name);
		return PyLong_AsLong(a_result);
	}


	PyObject* xen_py_fn_game_draw_texture(PyObject *self, PyObject *args) {
		PyObject* rect;
		int s_texture;
		if (!PyArg_ParseTuple(args, "Oi", &rect, &s_texture)) return NULL;

		int s_x = xen_py_read_attr_int(rect, "x");
		int s_y = xen_py_read_attr_int(rect, "y");
		int s_width = xen_py_read_attr_int(rect, "width");
		int s_height = xen_py_read_attr_int(rect, "height");

		XentuPythonMachine* m = XentuPythonMachine::GetInstance();
		auto r = m->GetRenderer();

		r->DrawTexture(s_texture, s_x, s_y, s_width, s_height);

		return PyBool_FromLong(1);
	}


	PyMethodDef xen_py_explain_module_game[] = {
		{"on", xen_py_fn_game_on, METH_VARARGS, "Handle an engine or custom event."},
		{"trigger", xen_py_fn_game_trigger, METH_VARARGS, "Trigger an event."},
		{"create_window", xen_py_fn_game_create_window, METH_VARARGS, "Create a new game window." },
		{"create_window_ex", xen_py_fn_game_create_window_ex, METH_VARARGS, "Create a new game window." },
		{"run", xen_py_fn_game_run, METH_VARARGS, "Begin running the game." },
		{"draw_texture", xen_py_fn_game_draw_texture, METH_VARARGS, "Draw a texture using a Rect" },
		{NULL, NULL, 0, NULL}
	};


	PyModuleDef xen_py_def_module_game = {
		PyModuleDef_HEAD_INIT, "game", NULL, -1, xen_py_explain_module_game,
		NULL, NULL, NULL, NULL
	};


	PyObject* xen_py_init_module_game(void) {
		return PyModule_Create(&xen_py_def_module_game);
	}


	/* ---- Game Module ------------------------------------------------------ */
}