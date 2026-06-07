#ifndef LUA_MACHINE_H
#define LUA_MACHINE_H

#include "../../lib/luna.hpp"

#include "../Machine.h"


//#include <unordered_map>

//using namespace std;

namespace xen
{
    class LuaMachine : public Machine
    {
    public:
        LuaMachine();
        ~LuaMachine();
        static LuaMachine* GetInstance();
        int Init();
        int Trigger(const string event_name);
        int Trigger(const string event_name, const string arg0);
        int Trigger(const string event_name, const int arg0);
        int Trigger(const string event_name, const float arg0);
        int On(const string event_name, const int callback_ref);

    private:
        lua_State* L;
        unordered_multimap<string, int> callbacks;
        static LuaMachine* instance;
    };
}

#endif //LUA_MACHINE_H
