#define PY_SSIZE_T_CLEAN

#include <SDL.h>
#include <Python.h>
#include <stdio.h>
#include <string>

#include "../vfs/XenVirtualFileSystem.h"
#include "../vfs/XenFileSystem.h"
#include "../vfs/XenZipFileSystem.h"
#include "PythonMachine.h"

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


	void xen_py_call_func(const char* function_name, const string arg0)
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
		PyObject *pArg0 = PyUnicode_FromString(arg0.c_str());
		PyObject_CallOneArg(pFunc, pArg0);
		Py_DECREF(pArg0);
		Py_DECREF(pFunc);
		Py_DECREF(pModule);
	}


	void xen_py_call_func(const char* function_name, const int arg0)
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
		PyObject *pArg0 = PyLong_FromSize_t(arg0);
		PyObject_CallOneArg(pFunc, pArg0);
		Py_DECREF(pArg0);
		Py_DECREF(pFunc);
		Py_DECREF(pModule);
	}


	void xen_py_call_func(const char* function_name, const float arg0)
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
		PyObject *pArg0 = PyFloat_FromDouble(arg0);
		PyObject_CallOneArg(pFunc, pArg0);
		Py_DECREF(pArg0);
		Py_DECREF(pFunc);
		Py_DECREF(pModule);
	}


	/* --- helpers ----------------------------------------------------------- */


	int xen_py_read_attr_int(PyObject* obj, char* name) {
		if (!PyObject_HasAttrString(obj, name)) return NULL;
		PyObject* a_result = PyObject_GetAttrString(obj, name);
		return PyLong_AsLong(a_result);
	}


	/* --- interop ----------------------------------------------------------- */


	PyObject* xen_py_interop_game_create_window(PyObject *self, PyObject *args)
	{
		PythonMachine* m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		int window_id = r->Init();
		return PyLong_FromLong(window_id);
	}
	
	PyObject* xen_py_interop_game_on(PyObject *self, PyObject *args)
	{
		char *s_event_name;
		char *s_callback;
		if (!PyArg_ParseTuple(args, "ss", &s_event_name, &s_callback)) {
			return NULL;
		}
		auto m = PythonMachine::GetInstance();
		m->On(s_event_name, s_callback);
		return PyBool_FromLong(1);
	}
	
	PyObject* xen_py_interop_game_trigger(PyObject *self, PyObject *args)
	{
		char *s_event_name;
		if (!PyArg_ParseTuple(args, "s", &s_event_name)) {
			return NULL;
		}
		// trigger the event.
		auto m = PythonMachine::GetInstance();
		m->Trigger(s_event_name);
		// return true
		return PyBool_FromLong(1);
	}
	
	PyObject* xen_py_interop_game_run(PyObject *self, PyObject *args)
	{
		auto m = PythonMachine::GetInstance();
		m->Run();
		return PyBool_FromLong(1);
	}
	
	PyObject* xen_py_interop_game_exit(PyObject *self, PyObject *args)
	{
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Exit();
		return PyBool_FromLong(1);
	}
	
	PyObject* xen_py_interop_assets_mount(PyObject *self, PyObject *args)
	{
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
	
	PyObject* xen_py_interop_assets_read_text_file(PyObject *self, PyObject *args)
	{
		char *s;
		if (!PyArg_ParseTuple(args, "s", &s)) {
			return NULL;
		}
		std::string result = vfs_get_global()->ReadAllText(s);
		return PyUnicode_FromString(result.data());
	}
	
	PyObject* xen_py_interop_assets_load_texture(PyObject *self, PyObject *args)
	{
		char *s;
		if (!PyArg_ParseTuple(args, "s", &s)) {
			return NULL;
		}
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		xen::VfsBufferResult res = vfs_get_global()->ReadAllData(s);
		printf("Bytes read: %s\n", std::to_string(res.length).c_str());
		int texture_id = r->LoadTexture(res.buffer, res.length);
		// todo: take the filename from s and load the texture.
		return PyLong_FromLong(texture_id);
	}
	
	PyObject* xen_py_interop_assets_load_font(PyObject *self, PyObject *args)
	{
		char *s;
		int font_size;
		if (!PyArg_ParseTuple(args, "si", &s, &font_size)) {
			return NULL;
		}
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		xen::VfsBufferResult res = vfs_get_global()->ReadAllData(s);
		printf("Bytes read: %s\n", std::to_string(res.length).c_str());
		int font_id = r->LoadFont(res.buffer, res.length, font_size);
		// todo: take the filename from s and load the font.
		return PyLong_FromLong(font_id);
	}
	
	PyObject* xen_py_interop_assets_create_textbox(PyObject *self, PyObject *args)
	{
		int x,y,w,h;
		if (!PyArg_ParseTuple(args, "iiii", &x, &y, &w, &h)) return NULL;
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		int textbox_id = r->CreateTextBox(x, y, w, h);
		return PyLong_FromLong(textbox_id);
	}
	
	PyObject* xen_py_interop_renderer_begin(PyObject *self, PyObject *args)
	{
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Begin();
		return PyLong_FromLong(1);
	}

	PyObject* xen_py_interop_renderer_clear(PyObject *self, PyObject *args)
	{
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Clear();
		return PyBool_FromLong(1);
	}
	
	PyObject* xen_py_interop_renderer_present(PyObject *self, PyObject *args)
	{
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Present();
		return PyBool_FromLong(1);
	}
	
	PyObject* xen_py_interop_renderer_draw_texture(PyObject *self, PyObject *args)
	{
		PyObject* rect;
		int texture, x, y, w, h;
		if (!PyArg_ParseTuple(args, "iiiii", &texture, &x, &y, &w, &h)) return NULL;
		PythonMachine* m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTexture(texture, x, y, w, h);
		return PyBool_FromLong(1);
	}
	
	PyObject* xen_py_interop_renderer_draw_sub_texture(PyObject *self, PyObject *args)
	{
		PyObject* rect;
		int texture, x, y, w, h, sx, sy, sw, sh;
		if (!PyArg_ParseTuple(args, "iiiiiiiii", &texture, &x, &y, &w, &h, &sx, &sy, &sw, &sh)) return NULL;
		PythonMachine* m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawSubTexture(texture, x, y, w, h, sx, sy, sw, sh);
		return PyBool_FromLong(1);
	}

	PyObject* xen_py_interop_renderer_draw_textbox(PyObject *self, PyObject *args)
	{
		PyObject* rect;
		int textbox_id;
		if (!PyArg_ParseTuple(args, "i", &textbox_id)) return NULL;
		PythonMachine* m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTextBox(textbox_id);
		return PyBool_FromLong(1);
	}
	
	PyObject* xen_py_interop_renderer_set_background(PyObject *self, PyObject *args)
	{
		char *hex;
		if (!PyArg_ParseTuple(args, "s", &hex)) {
			return NULL;
		}

		int r, g, b;
		sscanf(hex, "%02x%02x%02x", &r, &g, &b);
		printf("clear_color: %i,%i,%i (hex %s)\n", r, g, b, hex);
		auto machine = PythonMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetClearColor(r, g, b);

		return PyBool_FromLong(1);
	}

	PyObject* xen_py_interop_renderer_set_window_mode(PyObject *self, PyObject *args)
	{
		int mode;
		if (!PyArg_ParseTuple(args, "i", &mode)) {
			return NULL;
		}
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetWindowMode(static_cast<XenWindowMode>(mode));

		return PyBool_FromLong(1);
	}

	PyObject* xen_py_interop_renderer_set_position(PyObject *self, PyObject *args)
	{
		float x, y;
		if (!PyArg_ParseTuple(args, "ff", &x, &y)) {
			return NULL;
		}
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetPosition(x, y);
		return PyBool_FromLong(1);
	}

	PyObject* xen_py_interop_renderer_set_origin(PyObject *self, PyObject *args)
	{
		float x, y;
		if (!PyArg_ParseTuple(args, "ff", &x, &y)) {
			return NULL;
		}
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetOrigin(x, y);
		return PyBool_FromLong(1);
	}

	PyObject* xen_py_interop_renderer_set_rotation(PyObject *self, PyObject *args)
	{
		float angle;
		if (!PyArg_ParseTuple(args, "f", &angle)) {
			return NULL;
		}
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetRotation(angle);
		return PyBool_FromLong(1);
	}

	PyObject* xen_py_interop_renderer_set_scale(PyObject *self, PyObject *args)
	{
		float x, y;
		if (!PyArg_ParseTuple(args, "ff", &x, &y)) {
			return NULL;
		}
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetScale(x, y);
		return PyBool_FromLong(1);
	}
	
	PyObject* xen_py_interop_config_get_str(PyObject *self, PyObject *args) { return NULL; }
	
	PyObject* xen_py_interop_config_get_str2(PyObject *self, PyObject *args) { return NULL; }
	
	PyObject* xen_py_interop_config_get_bool(PyObject *self, PyObject *args) { return NULL; }
	
	PyObject* xen_py_interop_config_get_bool2(PyObject *self, PyObject *args) { return NULL; }
	
	PyObject* xen_py_interop_config_get_int(PyObject *self, PyObject *args) { return NULL; }
	
	PyObject* xen_py_interop_config_get_int2(PyObject *self, PyObject *args) { return NULL; }

	PyObject* xen_py_interop_textbox_set_text(PyObject *self, PyObject *args)
	{
		int textbox_id, font_id;
		char *text;
		if (!PyArg_ParseTuple(args, "iis", &textbox_id, &font_id, &text)) {
			return NULL;
		}
		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetTextBoxText(textbox_id, font_id, text);

		return PyBool_FromLong(1);
	}

	PyObject* xen_py_interop_textbox_set_color(PyObject *self, PyObject *args)
	{
		int textbox_id, font_id;
		char *hex;
		if (!PyArg_ParseTuple(args, "iis", &textbox_id, &font_id, &hex)) {
			return NULL;
		}

		int rr, g, b;
		sscanf(hex, "%02x%02x%02x", &rr, &g, &b);
		printf("font_color: %i,%i,%i (hex %s)", rr, g, b, hex);

		auto m = PythonMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetTextBoxColor(textbox_id, font_id, rr, g, b);

		return PyBool_FromLong(1);
	}

	PyObject* xen_py_interop_keyboard_key_down(PyObject *self, PyObject *args)
	{
		int key_code;
		if (!PyArg_ParseTuple(args, "i", &key_code)) {
			return NULL;
		}
		auto m = PythonMachine::GetInstance();
		auto i = m->GetInput();
		bool down = i->KeyDown(key_code);
		return PyBool_FromLong(down);
	}

	PyObject* xen_py_interop_keyboard_key_clicked(PyObject *self, PyObject *args)
	{
		int key_code;
		if (!PyArg_ParseTuple(args, "i", &key_code)) {
			return NULL;
		}
		auto m = PythonMachine::GetInstance();
		auto i = m->GetInput();
		bool down = i->KeyUp(key_code);
		return PyBool_FromLong(down);
	}

	/* ---- VFS Module ------------------------------------------------------- */



	PyMethodDef xen_py_explain_interop[] = {
		{"game_create_window",			xen_py_interop_game_create_window, METH_VARARGS, "Create window"},
		{"game_on",							xen_py_interop_game_on, METH_VARARGS, "Handle an engine or custom event."},
		{"game_trigger",					xen_py_interop_game_trigger, METH_VARARGS, "Trigger an event."},
		{"game_run",						xen_py_interop_game_run, METH_VARARGS, "Begin running the game."},
		{"game_exit",						xen_py_interop_game_exit, METH_VARARGS, "Exit the game."},
		{"assets_mount",					xen_py_interop_assets_mount, METH_VARARGS, "Mount a virtual path."},
		{"assets_read_text_file",		xen_py_interop_assets_read_text_file, METH_VARARGS, "Read a text file."},
		{"assets_load_texture",    	xen_py_interop_assets_load_texture, METH_VARARGS, "Load a texture."},
		{"assets_load_font",				xen_py_interop_assets_load_font, METH_VARARGS, "Load a font."},
		{"assets_create_textbox",  	xen_py_interop_assets_create_textbox, METH_VARARGS, "Create a textbox."},
		{"renderer_begin",				xen_py_interop_renderer_begin, METH_VARARGS, "Begin a render cycle."},
		{"renderer_clear",				xen_py_interop_renderer_clear, METH_VARARGS, "Clear the current render buffer."},
		{"renderer_present",				xen_py_interop_renderer_present, METH_VARARGS, "Present the current render cycle."},
		{"renderer_draw_texture",		xen_py_interop_renderer_draw_texture, METH_VARARGS, "Draw a texture."},
		{"renderer_draw_sub_texture", xen_py_interop_renderer_draw_sub_texture, METH_VARARGS, "Draw part of a texture."},
		{"renderer_draw_textbox",		xen_py_interop_renderer_draw_textbox, METH_VARARGS, "Draw a textbox."},
		{"renderer_set_background", 	xen_py_interop_renderer_set_background, METH_VARARGS, "Set the background color."},
		{"renderer_set_window_mode", 	xen_py_interop_renderer_set_window_mode, METH_VARARGS, "Set the window mode (fullscreen, windowed etc..)."},
		{"renderer_set_position", 		xen_py_interop_renderer_set_position, METH_VARARGS, "Set the current transform position (translation)."},
		{"renderer_set_origin",			xen_py_interop_renderer_set_origin, METH_VARARGS, "Set the current transform origin."},
		{"renderer_set_rotation",		xen_py_interop_renderer_set_rotation, METH_VARARGS, "Set the current transform rotation (in degrees)."},
		{"renderer_set_scale",			xen_py_interop_renderer_set_scale, METH_VARARGS, "Set the current transform scale (fractional)."},
		{"config_get_str",				xen_py_interop_config_get_str, METH_VARARGS, "Get a string setting"},
		{"config_get_str2",				xen_py_interop_config_get_str2, METH_VARARGS, "Get a string sub setting"},
		{"config_get_bool",				xen_py_interop_config_get_bool, METH_VARARGS, "Get a boolean setting"},
		{"config_get_bool2",				xen_py_interop_config_get_bool2, METH_VARARGS, "Get a boolean sub setting"},
		{"config_get_int",				xen_py_interop_config_get_int, METH_VARARGS, "Get a integer setting"},
		{"config_get_int2",				xen_py_interop_config_get_int2, METH_VARARGS, "Get a integer sub setting"},
		{"textbox_set_text",				xen_py_interop_textbox_set_text, METH_VARARGS, "Set text for a textbox."},
		{"textbox_set_color",			xen_py_interop_textbox_set_color, METH_VARARGS, "Set the color for a textbox."},
		{"keyboard_key_down",         xen_py_interop_keyboard_key_down, METH_VARARGS, "Check if a keyboard key is down."},
		{"keyboard_key_clicked",      xen_py_interop_keyboard_key_clicked, METH_VARARGS, "Check if a keyboard key is clicked."},
		{NULL, NULL, 0, NULL}
	};


	PyModuleDef xen_py_def_interop = {
		PyModuleDef_HEAD_INIT, "xentu", NULL, -1, xen_py_explain_interop,
		NULL, NULL, NULL, NULL
	};


	PyObject* xen_py_init_interop(void) {
		return PyModule_Create(&xen_py_def_interop);
	}


	/* ---- Game Module ------------------------------------------------------ */
}