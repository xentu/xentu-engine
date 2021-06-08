#ifndef XEN_GAME_CPP
#define XEN_GAME_CPP
#define GLFW_INCLUDE_NONE

#include <GLEW/GL/glew.h>
#include <GLFW3/glfw3.h>
#include <Windows.h>
#include <iostream>
#include <string>

#include <luna/luna.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "XentuGame.h"
#include "Helper.h"


// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name) {#name, &class::name}


namespace xen
{
	void xentuKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		std::cout << "Clicked: " << key << std::endl;
	}



	XentuGame::XentuGame(lua_State* L)
	{
		std::string base_path = XentuGame::get_current_directory();

		// not a true instance, so make sure to let the static variable know we've been created.
		instance = this;

		// consider removing state argument.
		std::cout << "Created instance of XentuGame." << std::endl;
		this->num = 99;
		this->shader = -1;
		this->window = 0;
		this->assets = new AssetManager(L);
		this->assets->base_path = base_path;
		this->renderer = new Renderer2D(L);
		this->audio = new AudioPlayer(L);
		this->input = new InputManager(L);
		this->initialized = false;
		this->m_closing = false;
		this->config = xen::Configuration::parse_file(base_path + "../../../data/Config.toml");
		this->viewport = new Viewport(this->config->m_viewport_width, this->config->m_viewport_height);
	}



	XentuGame::~XentuGame() {
		// clean up stuff that was created on constructor.
		delete assets;
		delete renderer;
		delete audio;
		delete input;
		delete viewport;

		/* cleanup shader */
		glDeleteProgram(shader);

		/* terminate glfw */
		//glfwDestroyWindow(window);
		glfwTerminate();

		std::cout << "Deleted instance of XentuGame." << std::endl;
	}



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
		glLinkProgram(program);
		glValidateProgram(program);

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



	int XentuGame::initialize(lua_State* L) {
		/* Make sure nobody calls initialize more than once */
		if (this->initialized == true)
			return -2;

		/* Setup glfw */
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		/* Initialize the glfw library */
		if (!glfwInit())
			return -1;

		/* Create a windowed mode window and its OpenGL context */
		//window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
		window = glfwCreateWindow(config->m_screen_width, config->m_screen_height, config->m_game_title.c_str(), NULL, NULL);
		std::cout << "Detected: " << config->m_screen_width << std::endl;
		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		/* Center the game */
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		center_window(window, monitor);

		/* Initialize GLEW so we have access to modern OpenGL methods. */
		if (glewInit() != GLEW_OK)
		    std::cout << "Error!" << std::endl;

		/* Announce the GL version. */
		std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

		/**/
		//glfwSetKeyCallback(window, xentuKeyCallback);

		/* Load our shaders from files. */
		std::string vertexShader = xen::Helper::read_text_file(assets->base_path + "../../../data/shaders/VertexShader.shader");
		std::string fragmentShader = xen::Helper::read_text_file(assets->base_path + "../../../data/shaders/FragmentShader.shader");

		shader = create_shader(vertexShader, fragmentShader);
		glUseProgram(shader);

		/* camera stuff */
		glViewport(0, 0, config->m_viewport_width, config->m_viewport_height);
		glm::mat4 proj = glm::ortho(0.0f, (float)config->m_viewport_width, (float)config->m_viewport_height, 0.0f);
		unsigned int transform_location = glGetUniformLocation(shader, "u_MVP");
		glUniformMatrix4fv(transform_location, 1, false, &proj[0][0]);
		
		/* texture prep */
		unsigned int texture_location = glGetUniformLocation(shader, "u_Texture");
		glUniform1f(texture_location, 0);

		// assets->load_texture("test", "C:/Users/conta/Pictures/511-256x256.jpg");
		// this->use_texture("test");
		//this->quad = Quad(0, 0, 100, 100);
		//this->quad.initialize();

		/* finished initializing */
		this->input->initialize(window);
		this->initialized = true;

		return 0;
	}



	void XentuGame::update(lua_State* L) {
		this->trigger(L, "update");
	}

    

	void XentuGame::draw(lua_State* L) {
		/* Render here. */
		glClear(GL_COLOR_BUFFER_BIT);

		//this->Renderer2D->clear();
		//this->Renderer2D->draw(this->sprite);

		/* Draw our vertex buffer. */
		//this->quad.draw();

		this->trigger(L, "draw");

		/* Swap front and back buffers. */
		glfwSwapBuffers(window);

		/* Poll for and process events. */
		glfwPollEvents();
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



	int XentuGame::on(lua_State* L)
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



	void XentuGame::__use_texture(int texture_id) {
		const Texture* texture = assets->get_texture(texture_id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->gl_texture_id);
	}



	int XentuGame::use_texture(lua_State* L)
	{
		int texture_id = lua_tointeger(L, -1);
		__use_texture(texture_id);
		return 1;
	}



	int XentuGame::log(lua_State* L)
	{
		std::string text = lua_tostring(L, -1);
		std::cout << text << std::endl;
		return 1;
	}

    

	bool XentuGame::is_running()
	{
		return !m_closing && !glfwWindowShouldClose(window);
	}



	int XentuGame::get_audio(lua_State* L)
	{
		Luna<xen::AudioPlayer>().push(L, this->audio);
		return 1;
	}



	int XentuGame::get_assets(lua_State* L)
	{
		Luna<xen::AssetManager>().push(L, this->assets);
		return 1;
	}



	int XentuGame::get_renderer(lua_State* L)
	{
		Luna<xen::Renderer2D>().push(L, this->renderer);
		return 1;
	}



	int XentuGame::get_input(lua_State* L)
	{
		Luna<xen::InputManager>().push(L, this->input);
		return 1;
	}



	int XentuGame::get_viewport(lua_State* L)
	{
		Luna<xen::Viewport>().push(L, this->viewport);
		return 1;
	}



	void XentuGame::poll_input()
	{
		glfwPollEvents();
	}



	int XentuGame::exit(lua_State* L)
	{
		if (m_closing == false) {
			m_closing = true;
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		return 1;
	}



	int XentuGame::debug_stack(lua_State* L)
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
					printf("> #%i number: %g\n",  i, lua_tonumber(L, i));  
					break;  
				default:  /* other values */  
					printf("> #%i is: %s\n",  i, lua_typename(L, t));  
					break;
			}
		}
		printf("--- Lua Stack Report ---\n");
		return 0;
	}



	std::string XentuGame::get_current_directory()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::string::size_type pos = std::string(buffer).find_last_of("\\/");
		return std::string(buffer).substr(0, pos) + "/";
	}



	const char xen::XentuGame::className[] = "XentuGame";



	const Luna<XentuGame>::PropertyType xen::XentuGame::properties[] = {
		{"audio", &XentuGame::get_audio, nullptr },
		{"assets", &XentuGame::get_assets, nullptr },
		{"sprites", &XentuGame::get_renderer, nullptr }, /* deprecated: use renderer instead */
		{"renderer", &XentuGame::get_renderer, nullptr },
		{"input", &XentuGame::get_input, nullptr },
		{"viewport", &XentuGame::get_viewport, nullptr },
		{0,0}
	};



	const Luna<XentuGame>::FunctionType xen::XentuGame::methods[] = {
		method(XentuGame, log),
		method(XentuGame, on),
		method(XentuGame, use_texture),
		method(XentuGame, exit),
		method(XentuGame, debug_stack),
		{0,0}
	};
}

#endif