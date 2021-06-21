#include <rang/rang.h>
#include <iostream>
#include <string>
#include <map>


using namespace std;
using namespace rang;


map<string, string> parsed_args;


int do_help()
{
    cout << "TODO: Help page goes here." << endl;
    return 0;
}


int do_path()
{
    char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	string path = string(buffer).substr(0, pos);
    cout << "Current path is: " << path << endl;
    return 0;
}


int do_new_game()
{
    cout << "TODO: New game command goes here." << endl;
    return 0;
}


int do_play_game()
{
    cout << "TODO: Play game command goes here." << endl;
    return 0;
}


int main(int arg_count, char* args[]) {
    cout << rang::fg::blue << "Xentu Game Engine SDK v" << XEN_SDK_VERSION << rang::fg::reset << endl;

    // if no args other than executed path are passed, let the user know how to find more information.
    if (arg_count == 1)
    {
        cout << rang::fg::red << "Please run xentusdk help for more information." << rang::fg::reset << endl;
        return 0;
    }

    // provide somewhere to store formatted args.
    string command_arg = args[1];

    // first arg must always be a command, so throw error if it isnt.
    if (command_arg[0] == '-')
    {
        cout << rang::fg::red << "Please provide a command as the first argument. For example: xentusdk help" << rang::fg::reset << endl;
        return 0;
    }    

    // build the map of arguments.
    if (arg_count > 2) {
        for (int i=2; i<arg_count; i++) {
            string key = args[i];
            if (i + 1 < arg_count) {
                parsed_args.insert(std::make_pair(key, args[i + 1]));
                i++; // advance 1 extra argument as we already used it here.
            }
            else {
                parsed_args.insert(std::make_pair(key, ""));
            }
        }
    }
    
    // handle the command
    if (command_arg == "help")
        return do_help();

    if (command_arg == "path")
        return do_path();
    
    if (command_arg == "new")
        return do_new_game();
    
    if (command_arg == "play")
        return do_play_game();
    
    cout << rang::fg::yellow << "Error unrecognised command '" << command_arg << "'." << rang::fg::reset << endl;
    return 0;
}


