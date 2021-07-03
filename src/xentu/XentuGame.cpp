#ifndef XEN_GAME_CPP
#define XEN_GAME_CPP
#define GLFW_INCLUDE_NONE

#include <GLEW/GL/glew.h>
#include <GLFW3/glfw3.h>
#include <iostream>
#include <string>

#include <luna/luna.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "XentuGame.h"
#include "Helper.h"

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	XentuGame::XentuGame(lua_State* L)
	{
		// not a true instance, so make sure to let the static variable know we've been created.
		instance = this;
		if (USE_PROXY_PATH) {
			this->base_path = xen::Helper::get_console_path();
		}
		else {
			this->base_path = xen::Helper::get_current_directory();
		}

		// consider removing state argument.
		std::cout << "Created instance of XentuGame." << std::endl;
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
		this->config = new xen::Configuration();
		this->viewport = new Viewport(320, 240);
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
			std::cout << "Tried to clean up a shader that does not exist." << std::endl;
		}
		

		/* terminate glfw */
		//glfwDestroyWindow(window);
		glfwTerminate();

		std::cout << "Deleted instance of XentuGame." << std::endl;
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
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
			std::cout << message << std::endl;
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
		glAttachShader(program, fs);

		glBindAttribLocation(program, 0, "position");
		glBindAttribLocation(program, 1, "texCoord");
		glBindAttribLocation(program, 2, "color");

		glLinkProgram(program);

		int linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (linked != true) {
			std::cout << "Failed to link shader." << std::endl;
		}

		int valid;
		glValidateProgram(program);
		glGetProgramiv(program, GL_VALIDATE_STATUS, &valid);
		if (valid != true) {
			std::cout << "Failed to validate linked shader." << std::endl;
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
			/* We need to explicitly ask for a 3.2 context on OS X */
			glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
			glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#else
			/* Setup glfw (Tries to use OpenGL ES 3.0) */
			//glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
			//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
			glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
			glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#endif
		
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(config->m_screen_width, config->m_screen_height, config->m_game_title.c_str(), NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			// export MESA_GL_VERSION_OVERRIDE=3.3
			std::cout << "Failed to create game window." << std::endl;
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		/* allow vysnc to be disabled via the configuration. */
		if (config->m_vsync == false) {
			glfwSwapInterval(0); // default = 1
		}

		/* Center the game */
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		center_window(window, monitor);

		/* if the config says fullscreen, do that here. */
		if (config->m_fullscreen) {
			this->set_fullscreen(true);
		}

		/* Initialize GLEW so we have access to modern OpenGL methods. */
		if (glewInit() != GLEW_OK)
			std::cout << "Error!" << std::endl;

		/* Announce the GL version. */
		std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

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


	bool XentuGame::pre_init()
	{
		/* find Game.lua */
		if (!xen::Helper::file_exists(base_path + "/game.lua")) {
			/* store a copy of the base path */
			std::string data_path = base_path;

			/* Possible locations for Game.lua */
			std::string possible_paths[5] = {
				"/data",
				"/../data",
				"/../../data",
				"/../../../data",
				"/../../../../data"
			};

			bool found = false;

			std::cout << "Base Path: " << base_path << std::endl;

			for (int i = 0; i < 5; i++) {
				data_path = base_path + possible_paths[i];
				std::cout << "Trying: " << base_path << possible_paths[i] + "/game.lua" << std::endl;
				if (xen::Helper::file_exists(data_path + "/game.lua")) {
					base_path = data_path;
					found = true;
					break;
				}
				if (xen::Helper::file_exists(data_path + "/Game.lua")) {
					base_path = data_path;
					found = true;
					break;
				}
			}

			if (found == false) {
				std::cout << "Failed to locate Game.lua, Xentu will now exit." << std::endl;
				return false;
			}
		}

		/* log the game folder path */
		std::string game_path = base_path + "/Game.lua";
		std::cout << "Located game at: " << game_path << std::endl;

		delete this->config; // get rid of old ref.
		this->assets->base_path = base_path;

		if (xen::Helper::file_exists(base_path + "/Config.toml")) {
			this->config = xen::Configuration::parse_file(base_path + "/Config.toml");
		}
		else {
			this->config = new xen::Configuration();
		}

		this->viewport = new Viewport(this->config->m_viewport_width, this->config->m_viewport_height);
		return true;
	}


#pragma endregion


#pragma region Public Methods


	void XentuGame::draw(lua_State* L)
	{
		/* Render here. */
		glClear(GL_COLOR_BUFFER_BIT);

		this->trigger(L, "draw");

		/* Swap front and back buffers. */
		glfwSwapBuffers(window);

		/* Poll for and process events. */
		glfwPollEvents();
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

	void XentuGame::update(lua_State* L) {
		this->trigger(L, "update");
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
			default:  /* other values */
				printf("> #%i is: %s\n", i, lua_typename(L, t));
				break;
			}
		}
		printf("--- Lua Stack Report ---\n");
		return 0;
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
		std::string file = lua_tostring(L, -1);
		std::string full_path = this->base_path + '/' + file;
		if (luaL_dofile(L, full_path.c_str()) == LUA_OK)
		{
			std::cout << "Loaded " << full_path << std::endl;
		}
		else
		{
			std::cout << "Failed to load " << full_path << std::endl;
			std::string errormsg = lua_tostring(L, -1);
			std::cout << errormsg << std::endl;
		}
		return 0;
	}

	int XentuGame::lua_trigger(lua_State* L)
	{
		std::string event = lua_tostring(L, -1);
		XentuGame* game = XentuGame::get_instance(L);
		game->trigger(L, event);
		return 0;
	}


#pragma endregion


	bool xen::XentuGame::USE_PROXY_PATH = false;

	const char xen::XentuGame::className[] = "XentuGame";

	const Luna<XentuGame>::PropertyType xen::XentuGame::properties[] = {
		{"audio", &XentuGame::get_audio, nullptr },
		{"assets", &XentuGame::get_assets, nullptr },
		{"sprites", &XentuGame::get_renderer, nullptr }, /* deprecated: use renderer instead */
		{"renderer", &XentuGame::get_renderer, nullptr },
		{"keyboard", &XentuGame::get_keyboard, nullptr },
		{"mouse", &XentuGame::get_mouse, nullptr },
		{"viewport", &XentuGame::get_viewport, nullptr },
		{0,0}
	};

	const Luna<XentuGame>::FunctionType xen::XentuGame::methods[] = {
		method(XentuGame, debug_stack, lua_debug_stack),
		method(XentuGame, exit, lua_exit),
		method(XentuGame, on, lua_on),
		method(XentuGame, require, lua_require),
		method(XentuGame, trigger, lua_trigger),
		{0,0}
	};
}

#endif