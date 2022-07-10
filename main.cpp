#include <memory>
#include <string>

#include "src/Xentu.h"
#include "src/machines/XentuPythonMachine.h"


int main(const int argc, const char *argv[])
{
    // Pick which machine we are going to use. A machine is an instance
    // of the xentu engine, that comes with a vfs, and comes in various
    // versions to support different scripting languages. In the example
    // below I am using the Python version.
    const std::unique_ptr<xen::XentuPythonMachine> 
        machine(new xen::XentuPythonMachine(argc, argv));

    // pass any global variables to the machine like this.
    machine->set_global("test", "hello");

    // startup the machine so that scripts can run. It's upto the script
    // to create a game window in this version of Xentu. The run() command
    // is essentially the game loop.
    int res = machine->run();

    // 3. at this point the game would have ended.
    XEN_LOG("\nMachine ended with code %i.", res);
    return res;
}