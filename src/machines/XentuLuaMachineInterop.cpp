#ifndef XEN_LUA_MACHINE_INTEROP_CPP
#define XEN_LUA_MACHINE_INTEROP_CPP


#include "luna/luna.hpp"

#include "XentuLuaMachine.h"
#include "XentuLuaMachineInterop.h"

#include "src/fs/XenVirtualFileSystem.h"
#include "src/fs/XenFileSystem.h"
#include "src/fs/XenZipFileSystem.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	XentuLuaMachineInterop::XentuLuaMachineInterop(lua_State* L)
	{
		printf("XentuLuaGame::XentuLuaGame called");
	}


	XentuLuaMachineInterop::~XentuLuaMachineInterop()
	{
		printf("XentuLuaGame::~XentuLuaGame called\n");
	}


	int XentuLuaMachineInterop::test(lua_State* L)
	{
		printf("Test called\n");
		return 0;
	}


	int XentuLuaMachineInterop::game_create_window(lua_State* L)
	{
		XEN_LOG("- Called game_create_window\n");
		XentuLuaMachine* m = XentuLuaMachine::GetInstance();
		auto r = m->GetRenderer();
		int window_id = r->CreateWindow();
		lua_pushinteger(L, 1);
		return 1;
	}

	int XentuLuaMachineInterop::game_on(lua_State* L)
	{
		XEN_LOG("- Called game_on\n");
		if (lua_isfunction(L, -1))
		{
			int functionRef = luaL_ref(L, LUA_REGISTRYINDEX);
			std::string callbackName = lua_tostring(L, -1);
			auto m = XentuLuaMachine::GetInstance();
			m->On(callbackName, functionRef);
		}

		return 0;
	}

	int XentuLuaMachineInterop::game_trigger(lua_State* L)
	{
		XEN_LOG("- Called game_trigger\n");
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		const std::string event = lua_tostring(L, -1);
		auto m = XentuLuaMachine::GetInstance();
		m->Trigger(event);
		return 0;
	}

	int XentuLuaMachineInterop::game_run(lua_State* L)
	{
		XEN_LOG("- Called game_run\n");
		auto m = XentuLuaMachine::GetInstance();
		m->Run();
		return 0;
	}

	int XentuLuaMachineInterop::game_exit(lua_State* L)
	{
		auto m = XentuLuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Exit();
		return 0;
	}

	int XentuLuaMachineInterop::geometry_create_rect(lua_State* L)
	{
		XEN_LOG("- Called geometry_create_rect\n");
		return 0;
	}

	int XentuLuaMachineInterop::assets_mount(lua_State* L)
	{
		XEN_LOG("- Called assets_mount\n");
		return 0;
	}

	int XentuLuaMachineInterop::assets_read_text_file(lua_State* L)
	{
		XEN_LOG("- Called assets_read_text_file\n");
		return 0;
	}

	int XentuLuaMachineInterop::assets_load_texture(lua_State* L)
	{
		XEN_LOG("- Called assets_load_texture\n");
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		auto path = lua_tostring(L, -1);
		auto m = XentuLuaMachine::GetInstance();
		auto r = m->GetRenderer();
		auto res = vfs_get_global()->ReadAllData(path);
		XEN_LOG("- Bytes read: %s\n", std::to_string(res.length).c_str());
		int texture_id = r->LoadTexture(res.buffer, res.length);
		lua_pushinteger(L, texture_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_font(lua_State* L)
	{
		XEN_LOG("- Called assets_load_font\n");
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "expecting exactly 2 arguments");
		}
		const std::string path = lua_tostring(L, -2);
		XEN_LOG("- Attempting to read font %s\n", path);
		auto font_size = lua_tointeger(L, -1);
		auto m = XentuLuaMachine::GetInstance();
		auto r = m->GetRenderer();
		auto res = vfs_get_global()->ReadAllData(path);
		XEN_LOG("- Bytes read: %s\n", std::to_string(res.length).c_str());
		int font_id = r->LoadFont(res.buffer, res.length, font_size);
		lua_pushinteger(L, font_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_create_textbox(lua_State* L)
	{
		XEN_LOG("- Called assets_create_textbox.\n");
		if (lua_gettop(L) != 4) {
			return luaL_error(L, "expecting exactly 4 arguments");
		}
		auto x = lua_tointeger(L, -4);
		auto y = lua_tointeger(L, -3);
		auto w = lua_tointeger(L, -2);
		auto h = lua_tointeger(L, -1);
		auto m = XentuLuaMachine::GetInstance();
		auto r = m->GetRenderer();
		int textbox_id = r->CreateTextBox(x, y, w, h);
		lua_pushinteger(L, textbox_id);
		return 1;
	}

	int XentuLuaMachineInterop::renderer_begin(lua_State* L)
	{
		XEN_LOG("- Called renderer_begin\n");
		return 0;
	}

	int XentuLuaMachineInterop::renderer_present(lua_State* L)
	{
		XEN_LOG("- Called renderer_present\n");
		return 0;
	}

	int XentuLuaMachineInterop::renderer_draw_texture(lua_State* L)
	{
		XEN_LOG("- Called renderer_draw_texture\n");
		if (lua_gettop(L) != 5) {
			return luaL_error(L, "expecting exactly 5 arguments");
		}
		int texture_id = lua_tointeger(L, -5);
		int x = lua_tointeger(L, -4);
		int y = lua_tointeger(L, -3);
		int w = lua_tointeger(L, -2);
		int h = lua_tointeger(L, -1);
		auto m = XentuLuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTexture(texture_id, x, y, w, h);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_draw_sub_texture(lua_State* L)
	{
		XEN_LOG("- Called renderer_draw_texture\n");
		if (lua_gettop(L) != 9) {
			return luaL_error(L, "expecting exactly 9 arguments");
		}
		int texture_id = lua_tointeger(L, -9);
		int x = lua_tointeger(L, -8);
		int y = lua_tointeger(L, -7);
		int w = lua_tointeger(L, -6);
		int h = lua_tointeger(L, -5);
		int sx = lua_tointeger(L, -4);
		int sy = lua_tointeger(L, -3);
		int sw = lua_tointeger(L, -2);
		int sh = lua_tointeger(L, -1);
		auto m = XentuLuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawSubTexture(texture_id, x, y, w, h, sx, sy, sw, sh);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_draw_textbox(lua_State* L)
	{
		XEN_LOG("- Called renderer_draw_texbox\n");
		int textbox_id = lua_tointeger(L, -1);
		auto m = XentuLuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTextBox(textbox_id);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_background(lua_State* L)
	{
		XEN_LOG("- Called renderer_set_background\n");
		auto hex = lua_tostring(L, -1);
		int r, g, b;
		sscanf(hex, "%02x%02x%02x", &r, &g, &b);

		printf("clear_color: 0x%x, 0x%x, 0x%x (hex %s)", r, g, b, hex);

		auto machine = XentuLuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetClearColor(r, g, b);

		return 0;
	}

	int XentuLuaMachineInterop::config_get_str(lua_State* L)
	{
		return 0;
	}

	int XentuLuaMachineInterop::config_get_str2(lua_State* L)
	{
		return 0;
	}

	int XentuLuaMachineInterop::config_get_bool(lua_State* L)
	{
		return 0;
	}

	int XentuLuaMachineInterop::config_get_bool2(lua_State* L)
	{
		return 0;
	}

	int XentuLuaMachineInterop::config_get_int(lua_State* L)
	{
		return 0;
	}

	int XentuLuaMachineInterop::config_get_int2(lua_State* L)
	{
		return 0;
	}

	int XentuLuaMachineInterop::textbox_set_text(lua_State* L)
	{
		if (lua_gettop(L) != 3) {
			return luaL_error(L, "expecting exactly 3 arguments");
		}
		auto textbox_id = lua_tointeger(L, -3);
		auto font_id = lua_tointeger(L, -2);
		std::string text = lua_tostring(L, -1);
		auto m = XentuLuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetTextBoxText(textbox_id, font_id, text.c_str());
		return 0;
	}

	int XentuLuaMachineInterop::keyboard_key_down(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		auto key_code = lua_tointeger(L, -1);
		auto m = XentuLuaMachine::GetInstance();
		auto r = m->GetRenderer();
		bool down = r->KeyDown(key_code);
		lua_pushboolean(L, down);
		return 1;
	}


	const char xen::XentuLuaMachineInterop::className[] = "XentuLuaMachineInterop";

	const Luna<XentuLuaMachineInterop>::PropertyType XentuLuaMachineInterop::properties[] = {
		{0,0}
	};

	const Luna<XentuLuaMachineInterop>::FunctionType XentuLuaMachineInterop::methods[] = {
		method(XentuLuaMachineInterop, test, test),
		method(XentuLuaMachineInterop, game_create_window, game_create_window),
		method(XentuLuaMachineInterop, game_on, game_on),
		method(XentuLuaMachineInterop, game_trigger, game_trigger),
		method(XentuLuaMachineInterop, game_run, game_run),
		method(XentuLuaMachineInterop, game_exit, game_exit),
		method(XentuLuaMachineInterop, geometry_create_rect, geometry_create_rect),
		method(XentuLuaMachineInterop, assets_mount, assets_mount),
		method(XentuLuaMachineInterop, assets_read_text_file, assets_read_text_file),
		method(XentuLuaMachineInterop, assets_load_texture, assets_load_texture),
		method(XentuLuaMachineInterop, assets_load_font, assets_load_font),
		method(XentuLuaMachineInterop, assets_create_textbox, assets_create_textbox),
		method(XentuLuaMachineInterop, renderer_begin, renderer_begin),
		method(XentuLuaMachineInterop, renderer_present, renderer_present),
		method(XentuLuaMachineInterop, renderer_draw_texture, renderer_draw_texture),
		method(XentuLuaMachineInterop, renderer_draw_sub_texture, renderer_draw_sub_texture),
		method(XentuLuaMachineInterop, renderer_draw_textbox, renderer_draw_textbox),
		method(XentuLuaMachineInterop, renderer_set_background, renderer_set_background),
		method(XentuLuaMachineInterop, config_get_str, config_get_str),
		method(XentuLuaMachineInterop, config_get_str2, config_get_str2),
		method(XentuLuaMachineInterop, config_get_bool, config_get_bool),
		method(XentuLuaMachineInterop, config_get_bool2, config_get_bool2),
		method(XentuLuaMachineInterop, config_get_int, config_get_int),
		method(XentuLuaMachineInterop, config_get_int2, config_get_int2),
		method(XentuLuaMachineInterop, textbox_set_text, textbox_set_text),
		method(XentuLuaMachineInterop, keyboard_key_down, keyboard_key_down),
		{0,0}
	};
}


#endif