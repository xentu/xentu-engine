#ifndef XEN_GAME_CPP
#define XEN_GAME_CPP

#include <iostream>
#include <filesystem>
#include <string>

#include <luna/luna.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "XentuGame.h"


extern "C" {
#include "realpath.h"
}


// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	XentuGame::XentuGame(lua_State* L)
	{
		instance = this;
		this->base_path = xen::Helper::get_current_directory();
		this->num = 99;
		this->shader = -1;
		this->window = 0;
		this->assets = new AssetManager(L);
		this->renderer = new Renderer2D(L);
		this->audio = new AudioPlayer(L);
		this->keyboard = new KeyboardManager(L);
		this->mouse = new MouseManager(L);
		this->initialized = false;
		this->m_closing = false;
		this->m_current_scene = nullptr;
		this->m_fullscreen = false;
		this->config = new xen::Configuration(L);
		this->viewport = new Viewport(320, 240);
		Advisor::logInfo("Created instance of XentuGame.");
	}


	XentuGame::~XentuGame()
	{
		// clean up stuff that was created on constructor.
		delete assets;
		delete renderer;
		delete audio;
		delete keyboard;
		delete mouse;
		delete viewport;

		/* cleanup shader */
		if (shader < 9999) {
			glDeleteProgram(shader);
		}
		else {
			Advisor::throwError("Tried to clean up a shader that does not exist.");
		}
		

		/* terminate glfw */
		//glfwDestroyWindow(window);
		glfwTerminate();

		Advisor::logInfo("Deleted instance of XentuGame.");
	}


#pragma region Static Utility Methods


	// Compiles an invidivual vertex or fragment shader.
	static unsigned int compile_shader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		/* handle errors */
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			#if defined(WIN32) || defined(_WIN32) 
			char* message = (char*)_malloca(length * sizeof(char));
			#else
			char* message = (char*)alloca(length * sizeof(char));
			#endif
			//
			glGetShaderInfoLog(id, length, &length, message);
			Advisor::throwError("Failed to compile ", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), " shader\n", message);
			glDeleteShader(id);
			return 0;
		}

		return id;
	}


	// Links and attaches a vertex and fragment shader pair.
	static unsigned int create_shader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glBindAttribLocation(program, 0, "i_position");
		glBindAttribLocation(program, 1, "i_texcoord");
		glBindAttribLocation(program, 2, "i_color");

		glAttachShader(program, fs);
		glLinkProgram(program);

		int linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (linked != GL_TRUE) {
			Advisor::throwError("Failed to link shader.");
		}

		// TODO: glBindVertexArray(VAO); should be done before validation to avoid a warning on the next part.

		int valid;
		glValidateProgram(program);
		glGetProgramiv(program, GL_VALIDATE_STATUS, &valid);
		if (valid != GL_TRUE) {
			GLint length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetProgramInfoLog(program, length, &length, &error[0]);
			Advisor::throwError(&error[0]);
		}

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}


	// Place a GLFW window at the centre of a screen.
	static void center_window(GLFWwindow* window, GLFWmonitor* monitor)
	{
		if (!monitor)
			return;

		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (!mode)
			return;

		int monitorX, monitorY;
		glfwGetMonitorPos(monitor, &monitorX, &monitorY);

		int windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		glfwSetWindowPos(window,
			monitorX + (mode->width - windowWidth) / 2,
			monitorY + (mode->height - windowHeight) / 2);
	}


#pragma endregion


#pragma region Initialization

	
	// Null, because instance will be initialized on demand.
	XentuGame* XentuGame::instance = 0;
	XentuGame* XentuGame::get_instance(lua_State* L)
	{
		if (instance == 0)
		{
			instance = new XentuGame(L);
		}

		return instance;
	}


	int XentuGame::initialize(lua_State* L, std::string default_vert, std::string default_frag)
	{
		/* Make sure nobody calls initialize more than once */
		if (this->initialized == true)
			return -2;

		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		/* Initialize the glfw library */
		if (!glfwInit())
			return -1;

		#ifdef __APPLE__
			/* We need to explicitly ask for a 3.3 context on OS X */
			glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#else
			/* Setup glfw (Tries to use OpenGL ES 3.0) */
			//glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
			glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
			//glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#endif
		
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(config->m_screen_width, config->m_screen_height, config->m_game_title.c_str(), NULL, NULL);
		if (!window)
		{
			const char* e_description;
			int e_code = glfwGetError(&e_description);
			std::cout << "- glfw error: " << std::to_string(e_code) << " : " << e_description << std::endl;

			glfwTerminate();
			// export MESA_GL_VERSION_OVERRIDE=3.3
			Advisor::throwError("Failed to create game window.");
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		/* allow vysnc to be disabled via the configuration. */
		if (config->m_vsync == false) {
			glfwSwapInterval(0);
		}
		else {
			glfwSwapInterval(1);
		}

		/* Center the game */
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		center_window(window, monitor);

		/* if the config says fullscreen, do that here. */
		if (config->m_fullscreen) {
			this->set_fullscreen(true);
		}

		/* Initialize GLEW so we have access to modern OpenGL methods. */
		if (glewInit() != GLEW_OK) {
			Advisor::throwError("Failed to initialize glew!");
			return 0;
		}
			
		/* Announce the GL version. */
		Advisor::logInfo("GL Version: ", glGetString(GL_VERSION));

		/* Load our shaders. */
		std::string vertexShader = default_vert;
		if (xen::Helper::file_exists(assets->base_path + "/shaders/VertexShader.shader")) {
			vertexShader = xen::Helper::read_text_file(assets->base_path + "/shaders/VertexShader.shader");
		}

		std::string fragmentShader = default_frag;
		if (xen::Helper::file_exists(assets->base_path + "/shaders/FragmentShader.shader")) {
			fragmentShader = xen::Helper::read_text_file(assets->base_path + "/shaders/FragmentShader.shader");
		}

		shader = create_shader(vertexShader, fragmentShader);
		glUseProgram(shader);

		/* finished initializing */
		this->keyboard->initialize(window);
		this->mouse->initialize(window);
		this->initialized = true;

		return 0;
	}


	int XentuGame::pre_init(lua_State* L)
	{
		bool found = false;

		/* find Game.lua */
		if (xen::Helper::file_exists(base_path + "/game.lua")) {
			found = true;
			Advisor::logInfo("Located game at: ", base_path, "/game.lua");
		}
		else {
			/* Possible locations for game.lua */
			const int possible_path_count = 5;
			std::string possible_paths[possible_path_count] = {
				"/data",
				"/../data",
				"/../../data",
				"/../../../data",
				"/../../../../data"
			};
			// TODO: remove searching directories above the base_path as it's an anti-pattern.

			Advisor::logInfo("Base Path: ", base_path);

			/* iterate through the possibles to find a match */
			std::string possible_path = base_path;
			for (int i = 0; i < possible_path_count; i++) {
				possible_path = base_path + possible_paths[i];
				//Advisor::logInfo("Trying: ", possible_path, "/game.lua");
				if (xen::Helper::file_exists(possible_path + "/game.lua")) {
					base_path = possible_path;
					found = true;
					Advisor::logInfo("Located game at: ", possible_path, "/game.lua");
					this->set_base_path(base_path);
					break;
				}
			}
		}

		/* only proceed if the game.lua file was found */
		if (found == false) {
			Advisor::throwError("Failed to locate Game.lua, Xentu will now exit.");
			return -1;
		}

		/* share the working base_path with the assets system */
		this->assets->base_path = base_path;

		/* attempt to load the config file */
		if (xen::Helper::file_exists(base_path + "/config.toml")) {
			delete this->config; // get rid of old ref.
			this->config = xen::Configuration::parse_file(L, base_path + "/config.toml");
		}

		/* set viewport dimensions based on the config settings */
		this->viewport->width = this->config->m_viewport_width;
		this->viewport->height = this->config->m_viewport_height;

		return 0;
	}


#pragma endregion


#pragma region Public Methods


	void XentuGame::draw(lua_State* L)
	{
		/* Render here. */
		glClear(GL_COLOR_BUFFER_BIT);

		this->trigger(L, "draw");

		if (this->m_current_scene != nullptr) {
			this->m_current_scene->trigger(L, "draw");
		}

		/* Swap front and back buffers. */
		glfwSwapBuffers(window);

		/* Poll for and process events. */
		glfwPollEvents();
	}

	void XentuGame::set_base_path(std::string base_path)
	{
		char resolved_path[512]; 
        realpath(base_path.c_str(), resolved_path);
		this->base_path = resolved_path; // std::string( p );
	}

	std::string XentuGame::get_base_path()
	{
		return this->base_path;
	}

	bool XentuGame::is_running()
	{
		return !m_closing && !glfwWindowShouldClose(window);
	}

	void XentuGame::poll_input()
	{
		glfwPollEvents();
	}

	void XentuGame::set_fullscreen(bool fullscreen)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (fullscreen) {
			glfwSetWindowMonitor(window, monitor, 0, 0, config->m_screen_width, config->m_screen_height, mode->refreshRate);
		}
		else {
			glfwSetWindowMonitor(window, nullptr, 0, 0, config->m_screen_width, config->m_screen_height, mode->refreshRate);
		}
		m_fullscreen = fullscreen;
	}

	void XentuGame::trigger(lua_State* L, std::string callbackName)
	{
		int functionRef = this->callbacks[callbackName];
		if (functionRef > 0)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
			lua_call(L, 0, 0);
		}
	}

	void XentuGame::trigger_integer(lua_State* L, std::string callbackName, lua_Integer data)
	{
		int functionRef = this->callbacks[callbackName];
		if (functionRef > 0)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
			lua_pushinteger(L, data);
			lua_call(L, 1, 0);
		}
	}

	void XentuGame::trigger_number(lua_State* L, std::string callbackName, lua_Number data)
	{
		int functionRef = this->callbacks[callbackName];
		if (functionRef > 0)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
			lua_pushnumber(L, data);
			lua_call(L, 1, 0);
		}
	}

	void XentuGame::update(lua_State* L, const float delta) {
		this->trigger_number(L, "update", delta);

		if (this->m_current_scene != nullptr) {
			this->m_current_scene->trigger_number(L, "update", delta);
			// TODO: add trigger_integer to scene class.
		}
	}


	void XentuGame::set_scene(lua_State* L, XentuScene* scene) {
		// if there is a previous scene, let it know it's loosing focus.
		if (this->m_current_scene != nullptr) {
			this->m_current_scene->trigger(L, "hidden");
		}
		// set the scene.
		this->m_current_scene = scene;
		if (this->m_current_scene != nullptr) {
			this->m_current_scene->trigger(L, "shown");
			if (this->m_current_scene->m_initialized == false) {
				this->m_current_scene->trigger(L, "init");
			}
		}
	}


#pragma endregion
	

#pragma region Lua Accessible Methods


	int XentuGame::get_assets(lua_State* L)
	{
		Luna<xen::AssetManager>().push(L, this->assets);
		return 1;
	}

	int XentuGame::get_audio(lua_State* L)
	{
		Luna<AudioPlayer>().push(L, this->audio);
		return 1;
	}

	int XentuGame::get_keyboard(lua_State* L)
	{
		Luna<xen::KeyboardManager>().push(L, this->keyboard);
		return 1;
	}

	int XentuGame::get_mouse(lua_State* L)
	{
		Luna<xen::MouseManager>().push(L, this->mouse);
		return 1;
	}

	int XentuGame::get_renderer(lua_State* L)
	{
		Luna<xen::Renderer2D>().push(L, this->renderer);
		return 1;
	}

	int XentuGame::get_viewport(lua_State* L)
	{
		Luna<xen::Viewport>().push(L, this->viewport);
		return 1;
	}

	int XentuGame::get_config(lua_State* L)
	{
		Luna<xen::Configuration>().push(L, this->config);
		return 1;
	}

	int XentuGame::lua_debug_stack(lua_State* L)
	{
		int i;
		int top = lua_gettop(L);
		printf("--- Lua Stack Report ---\n");
		printf("total in stack: %d\n", top);
		for (i = 1; i <= top; i++)
		{  /* repeat for each level */
			int t = lua_type(L, i);
			switch (t) {
			case LUA_TSTRING:  /* strings */
				printf("> #%i string: '%s'\n", i, lua_tostring(L, i));
				break;
			case LUA_TBOOLEAN:  /* booleans */
				printf("> #%i boolean %s\n", i, lua_toboolean(L, i) ? "true" : "false");
				break;
			case LUA_TNUMBER:  /* numbers */
				printf("> #%i number: %g\n", i, lua_tonumber(L, i));
				break;
			case LUA_TTABLE:
				printf("> #%i table.", i);
				break;
			default:  /* other values */
				printf("> #%i is: %s\n", i, lua_typename(L, t));
				break;
			}
		}
		printf("--- Lua Stack Report ---\n");
		return 0;
	}


	int XentuGame::lua_get_path(lua_State* L)
	{
		std::string path = this->get_base_path();
		lua_pushstring(L, path.c_str());
		return 1;
	}


	int XentuGame::lua_exit(lua_State* L)
	{
		if (m_closing == false) {
			m_closing = true;
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		return 1;
	}

	int XentuGame::lua_on(lua_State* L)
	{
		if (lua_isfunction(L, -1))
		{
			int functionRef = luaL_ref(L, LUA_REGISTRYINDEX);
			std::string callbackName = lua_tostring(L, -1);
			this->callbacks.insert(std::make_pair(callbackName, functionRef));
			return 1;
		}

		return 0;
	}

	int XentuGame::lua_require(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		const std::string file = lua_tostring(L, -1);
		const std::string full_path = this->base_path + '/' + file;
		if (luaL_dofile(L, full_path.c_str()) == LUA_OK)
		{
			Advisor::logInfo("Loaded ", full_path);
		}
		else
		{
			std::string errormsg = lua_tostring(L, -1);
			Advisor::throwError("Failed to load ", full_path, ": ", errormsg);
		}
		return 0;
	}

	int XentuGame::lua_trigger(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		const std::string event = lua_tostring(L, -1);
		this->trigger(L, event);
		return 0;
	}

	int XentuGame::lua_trigger_with(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "expecting exactly 2 arguments");
		}
		const std::string event = lua_tostring(L, -2);
		int functionRef = this->callbacks[event];
		if (functionRef > 0)
		{
			int t = lua_type(L, -1);
			if (t == LUA_TSTRING) {
				const auto str_arg = lua_tostring(L, -1);
				lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
				lua_pushstring(L, str_arg);
				lua_call(L, 1, 0);
			}
			else if (t == LUA_TBOOLEAN) {
				const auto bool_arg = lua_toboolean(L, -1);
				lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
				lua_pushboolean(L, bool_arg);
				lua_call(L, 1, 0);
			}
			else if (t == LUA_TNUMBER) {
				const auto num_arg = lua_tonumber(L, -1);
				lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
				lua_pushnumber(L, num_arg);
				lua_call(L, 1, 0);
			}
			else {
				Advisor::throwError("Tried to trigger an event with an unsupported data argument " + std::to_string(t) + ".");
			}
		}

		return 0;
	}


	int XentuGame::lua_set_scene(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		// deal with double reference evil >:)
		XentuScene** scene = static_cast<XentuScene**>(lua_touserdata(L, -1));
		XentuGame* game = XentuGame::get_instance(L);
		game->set_scene(L, *scene);
		return 0;
	}


	int XentuGame::lua_set_fullscreen(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		bool fs = lua_toboolean(L, -1);
		this->set_fullscreen(fs);
		return 0;
	}


	int XentuGame::lua_get_fullscreen(lua_State* L)
	{
		lua_pushboolean(L, m_fullscreen);
		return 1;
	}


#pragma endregion

	const char xen::XentuGame::className[] = "XentuGame";

	const Luna<XentuGame>::PropertyType xen::XentuGame::properties[] = {
		{"audio", &XentuGame::get_audio, nullptr },
		{"assets", &XentuGame::get_assets, nullptr },
		{"renderer", &XentuGame::get_renderer, nullptr },
		{"keyboard", &XentuGame::get_keyboard, nullptr },
		{"mouse", &XentuGame::get_mouse, nullptr },
		{"viewport", &XentuGame::get_viewport, nullptr },
		{"config", &XentuGame::get_config, nullptr },
		{"path", &XentuGame::lua_get_path, nullptr },
		{"fullscreen", &XentuGame::lua_get_fullscreen, nullptr },
		{0,0}
	};

	const Luna<XentuGame>::FunctionType xen::XentuGame::methods[] = {
		method(XentuGame, debug_stack, lua_debug_stack),
		method(XentuGame, exit, lua_exit),
		method(XentuGame, on, lua_on),
		method(XentuGame, require, lua_require),
		method(XentuGame, trigger, lua_trigger),
		method(XentuGame, trigger_with, lua_trigger_with),
		method(XentuGame, set_scene, lua_set_scene),
		method(XentuGame, set_fullscreen, lua_set_fullscreen),
		{0,0}
	};
}

#endif