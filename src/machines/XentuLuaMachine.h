#pragma once

#include <string>
#include <unordered_map>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

using namespace std;

namespace xen
{
	class XentuLuaMachine : public XentuMachine
	{
		public:
			XentuLuaMachine(int argc, char *argv[], XentuConfig* config);
			~XentuLuaMachine();
			static XentuLuaMachine* GetInstance();
			int Init();
			int Trigger(const string event_name);
			int Trigger(const string event_name, const string arg0);
			int Trigger(const string event_name, const int arg0);
			int Trigger(const string event_name, const float arg0);
			int On(const string event_name, const int callback_ref);
		
		private:
			lua_State* L;
			unordered_map<string, int> callbacks;
			static XentuLuaMachine* instance;
	};
}