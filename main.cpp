#include <memory>
#include <string>

#include "src/machines/XentuLuaMachine.h"
#include "src/machines/XentuPythonMachine.h"

#include "src/fs/XenVirtualFileSystem.h"
#include "src/fs/XenFileSystem.h"

#include "src/fs/XenNativeFileSystem.h"
#include "src/fs/XenZipFileSystem.h"

using std::unique_ptr;


/**
 * Demonstrates how to setup the vfs using a native filesystem and write a text file.
 */
void example_write_text_file()
{
    xen::vfs_initialize();

    xen::XenFileSystemPtr root_fs(new xen::XenNativeFileSystem("./"));
    root_fs->Initialize();

    xen::XenVirtualFileSystemPtr vfs = xen::vfs_get_global();
    vfs->AddFileSystem("/", root_fs);

    xen::XenFilePtr nativeFile = vfs->OpenFile(xen::XenFileInfo("/newFile.txt"), xen::XenFile::ReadWrite);
    if (nativeFile && nativeFile->IsOpened())
    {
        std::string data = "The quick brown fox jumps over the lazy dog.";
        char char_array[255];
        int data_len = data.length();
        data.copy(char_array, data_len);
    	nativeFile->Write(reinterpret_cast<uint8_t*>(char_array), data_len);
    	nativeFile->Close();
    }

    xen::vfs_shutdown();
}


/**
 * Demonstrates how to setup the vfs using a zip filesystem and read a text file.
 */
void example_read_zip_file()
{
    xen::vfs_initialize();

    //xen::XenFileSystemPtr root_fs(new xen::XenNativeFileSystem("./"));
    xen::XenFileSystemPtr zip_fs(new xen::XenZipFileSystem("./assets/test.zip", "/"));

    //root_fs->Initialize();
    zip_fs->Initialize();

    xen::XenVirtualFileSystemPtr vfs = xen::vfs_get_global();
    //vfs->AddFileSystem("/f", root_fs);
    vfs->AddFileSystem("/zip/", zip_fs);

    xen::XenFilePtr zipFile = vfs->OpenFile(xen::XenFileInfo("/zip/file01.txt"), xen::XenFile::In);
    if (zipFile && zipFile->IsOpened())
    {
        char data[256];
        printf("\n");
        zipFile->Read(reinterpret_cast<uint8_t*>(data), 256);
        printf("%s\n", data);
    }
    else {
        printf("\nCouldnt load file");
    }

    xen::vfs_shutdown();
}



int main(const int argc, const char *argv[])
{
    // Pick which machine we are going to use. A machine is an instance
    // of the xentu engine, that comes with a vfs, and comes in various
    // versions to support different scripting languages. In the example
    // below I am using the Python version.

    // unique_ptr<xen::XentuLuaMachine> env(new xen::XentuLuaMachine());
    const unique_ptr<xen::XentuPythonMachine> machine(new xen::XentuPythonMachine);

    // 1. pass some global variables on the machine before it starts.
    machine->set_global("argc", std::to_string(argc));
    machine->set_global("exe_path", std::string(argv[0]));

    // 2. startup the machine so that scripts can run. It's upto the script
    // to create a game window in this version of Xentu. And the run() command
    // is essentially the game loop.
    int res = machine->run();


    // vfs test -----------------------------------
    example_read_zip_file();


    // 3. at this point the game would have ended.
    printf("\nMachine ended with code %i.", res);
    return res;
}