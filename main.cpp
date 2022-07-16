#include <memory>
#include <string>

#include "src/Xentu.h"
//#include "src/machines/XentuJavaScriptMachine.h"
#include "src/machines/XentuLuaMachine.h"
#include "src/machines/XentuPythonMachine.h"

using MACHINE_PY_PTR = std::unique_ptr<xen::XentuMachine>;
//using MACHINE_PTR_STR = std::unique_ptr<xen::XentuMachine<std::string>>;

int main(const int argc, const char *argv[])
{
    //const MACHINE_PTR js_machine(new xen::XentuJavaScriptMachine(argc, argv));
    //js_machine->run("/test.js");

    //const MACHINE_PTR_LUA lua_machine(new xen::XentuLuaMachine(argc, argv));
    //lua_machine->run("/test.lua");

    //const MACHINE_PY_PTR py_machine(new xen::XentuPythonMachine(argc, argv));
    //py_machine->run("/test.py");

    // 3. at this point the game would have ended.
    XEN_LOG("\nEnded with code %i.", res);
    return 0;
}