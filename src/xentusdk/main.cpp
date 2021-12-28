#include <rang/rang.h>
#include <iostream>
#include <string>
#include <map>

#if defined(WIN32) || defined(_WIN32) 
#define PATH_SEPARATOR '\\' 
#else 
#define PATH_SEPARATOR '/'
#include <sys/stat.h>
#endif

#include "templates.h"


#include "functions.cpp"
#if defined(WIN32) || defined(_WIN32) 
#include <windows.h>
#endif


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
    cout << "Current path is: " << get_console_path() << endl;
    cout << "SDK path is: " << get_sdk_path() << endl;
    return 0;
}


int do_new_game()
{
    string m_name = parsed_args.count("--name") ? parsed_args["--name"] : "";
    string m_author = parsed_args.count("--author") ? parsed_args["--author"] : "";
    string m_base = get_console_path();
    string s_base = get_sdk_path();

    if (m_name.length() == 0) {
        cout << "Please enter a name for the game: ";
        getline(cin, m_name);
    }

    if (m_author.length() == 0) {
        cout << "Please enter your author name: ";
        getline(cin, m_author);
    }

    const string m_name_replace("{title}");
    const string m_author_replace("{author}");


    // process the config.toml file
    string config_toml = replace_first(toml_template, m_name_replace, m_name);
    config_toml = replace_first(config_toml, m_author_replace, m_author);
    string config_toml_path = m_base + PATH_SEPARATOR + "config.toml";
    write_text_file(config_toml_path, config_toml);

    // process the game.lua file
    string game_lua = replace_first(lua_template, m_name_replace, m_name);
    string game_lua_path = m_base + PATH_SEPARATOR + "game.lua";
    write_text_file(game_lua_path, game_lua);

    // make the directories
    string data_path = m_base + PATH_SEPARATOR + "data";

    #if defined(WIN32) || defined(_WIN32) 
        mkdir(data_path.c_str());
    #else
        mkdir(data_path.c_str(), 0700);
    #endif
    
    // copy the base image.
    string image_from = s_base + PATH_SEPARATOR + "logo.png";
    string image_to = m_base + PATH_SEPARATOR + "logo.png";
    copy_file(image_from.c_str(), image_to.c_str());

    cout << "Game prepared. Type \"xentusdk play\" to startup your game!" << endl;
    return 0;
}


int do_clone_game(string template_name) {
    if (template_name.length() == 0) {
        cout << rang::fg::red << "Error, please provide a valid template name!" << endl;
        return -1;
    }

    string file = "M:\\GitHub\\xentu\\xentu-engine\\out\\build\\gcc\\templates\\" + template_name + ".zip";

    if (!file_exists(file)) {
        cout << rang::fg::red << "Error, cant load template named " << file << "!" << endl;
        return -2;
    }

    string m_base = get_console_path();

    cout << "Template: " << file << endl;
    cout << "Destination: " << m_base << endl;
    unzip_template(file, m_base);

    cout << "Game prepared. Type \"xentusdk play\" to startup your game!" << endl;
    return 0;
}


int do_play_game()
{
    system("xentu .");
    return 0;
}


int do_install()
{
    cout << "Installing..." << endl;
    #if defined(WIN32) || defined(_WIN32) 

    const string env_p = get_perm_env_variable("PATH");
    if (env_p.length() == 0) {
        cout << "Failed to read environment path variable." << endl;
        return 0;
    }
    const string env_p2 = (string)env_p + ";" + get_sdk_path();
    set_perm_env_variable("PATH", env_p2.c_str());
    cout << "Environment path set." << endl;
    #else
        cout << "Skipped env set, not on windows." << endl;
    #endif
    return 0;
}


int main(int arg_count, char* args[]) {
    cout << rang::fg::reset << "Xentu Game Engine SDK v" << XEN_SDK_VERSION << rang::fg::reset << endl;

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
    
    // handle the command.
    if (command_arg == "help") return do_help();
    if (command_arg == "path") return do_path();
    if (command_arg == "new")  return do_new_game();
    if (command_arg == "clone") return do_clone_game(arg_count > 2 ? args[2] : "");
    if (command_arg == "play") return do_play_game();
    if (command_arg == "install") return do_install();
    if (command_arg == "version") return 0;
    
    // default output.
    cout << rang::fg::yellow << "Error unrecognised command '" << command_arg << "'." << rang::fg::reset << endl;
    return 0;
}