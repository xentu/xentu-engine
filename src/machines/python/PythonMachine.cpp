#if XEN_PY
#define PY_SSIZE_T_CLEAN

#include "../../Globals.h"
#include "../../StringUtils.h"
#include "../../Config.h"
#include "../../Machine.h"
#include "../../vfs/XenVirtualFileSystem.h"
#include "PythonMachine.h"
#include "PythonMachineScripts.h"

namespace xen
{
	PythonMachine::PythonMachine(int argc, char *argv[], Config* config)
	:	Machine::Machine(argc, argv, config)
	{
		// grab the command line arguments in a format python likes.
		for (int i=0; i<argc && i<MAX_ARGV; i++) {
			arg_values_py[i] = Py_DecodeLocale(argv[i], NULL);
		}

		instance = this;
		
		// get a program name from the args.
		m_program = arg_values_py[0];
		Py_SetProgramName(m_program);

		// load the xen module.
		PyImport_AppendInittab("xentu", &xen_py_init_interop);

		// initialize python, passing the args.
		Py_Initialize();
		PySys_SetArgv(arg_count, (wchar_t **)arg_values_py);

		// load in our custom import loader.
		for (int i=0; i<xen_py_script_init_c; i++) {
			PyRun_SimpleString(xen_py_script_init[i]);
		}
	}


	PythonMachine* PythonMachine::instance = 0;
	PythonMachine* PythonMachine::GetInstance()
	{
		if (instance == 0)
		{
			printf("ERROR");
			exit(123);
		}
		return instance;
	}


	int PythonMachine::Init()
	{
		// load some python code.
		auto config = this->GetConfig();
		std::string py_code = vfs_get_global()->ReadAllText(config->entry_point);

		// add primary event bindings if defined in the user code.
		if (StringUtils::Contains(py_code, "def init(")) py_code += "\r\ngame.on('init', 'init')";
		if (StringUtils::Contains(py_code, "def update(")) py_code += "\r\ngame.on('update', 'update')";
		if (StringUtils::Contains(py_code, "def draw(")) py_code += "\r\ngame.on('draw', 'draw')";
		if (StringUtils::Contains(py_code, "def key_down(")) py_code += "\r\ngame.on('key_down', 'key_down')";
		if (StringUtils::Contains(py_code, "def key_click(")) py_code += "\r\ngame.on('key_click', 'key_click')";
		if (StringUtils::Contains(py_code, "def window_changed(")) py_code += "\r\ngame.on('window_changed', 'window_changed')";

		if (StringUtils::Contains(py_code, "def init_callback(")) py_code += "\r\ngame.on('init', 'init_callback')";
		if (StringUtils::Contains(py_code, "def update_callback(")) py_code += "\r\ngame.on('update', 'update_callback')";
		if (StringUtils::Contains(py_code, "def draw_callback(")) py_code += "\r\ngame.on('draw', 'draw_callback')";
		if (StringUtils::Contains(py_code, "def key_down_callback(")) py_code += "\r\ngame.on('key_down', 'key_down_callback')";
		if (StringUtils::Contains(py_code, "def key_click_callback(")) py_code += "\r\ngame.on('key_click', 'key_click_callback')";
		if (StringUtils::Contains(py_code, "def window_changed_callback(")) py_code += "\r\ngame.on('window_changed', 'window_changed_callback')";

		py_code += "\r\ngame.run()\n\n";

		// run some python code.
		return PyRun_SimpleString(py_code.c_str());
	}


	int PythonMachine::Trigger(const std::string event_name)
	{
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			xen_py_call_func(it->second.c_str());
		}
		return 0;
	}


	int PythonMachine::Trigger(const string event_name, const string arg0)
	{
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			xen_py_call_func(it->second.c_str(), arg0);
		}
		return 0;
	}


	int PythonMachine::Trigger(const string event_name, const int arg0)
	{
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			xen_py_call_func(it->second.c_str(), arg0);
		}
		return 0;
	}

	
	int PythonMachine::Trigger(const string event_name, const float arg0)
	{
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			xen_py_call_func(it->second.c_str(), arg0);
		}
		return 0;
	}


	int PythonMachine::On(const std::string event_name, const std::string callback_ref)
	{
		auto pair = std::make_pair(event_name, callback_ref);
		this->callbacks.insert(pair);
		return 1;
	}


	PythonMachine::~PythonMachine()
	{
		// tidy up python before closing.
		if (Py_FinalizeEx() < 0) {
			exit(120);
		}
		instance = nullptr;
		PyMem_RawFree(m_program);
	}
}

#endif