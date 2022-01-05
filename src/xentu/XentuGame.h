#ifndef XEN_GAME
#define XEN_GAME

#include <unordered_map>
#include <luna/luna.hpp>

#include "XentuScene.h"
#include "audio/AudioPlayer.h"
#include "filesystem/AssetManager.h"
#include "filesystem/Configuration.h"
#include "graphics/Renderer2D.h"
#include "graphics/Viewport.h"
#include "input/KeyboardManager.h"
#include "input/MouseManager.h"
#include "utilities/Helper.h"
#include "utilities/Advisor.h"


namespace xen {
	class XentuGame
	{
	public:
		/// <summary>
		/// Creates a new xentu instance.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		XentuGame(lua_State* L);
		~XentuGame(void);


#pragma region Initialization


		/// <summary>
		/// Initializes the game window and OpenGL.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		/// <returns>Exit code, 0 unless something went wrong.</returns>
		int initialize(lua_State* L, std::string default_vert, std::string default_frag);

		/// <summary>
		/// Gets the single instance available of this class during the application lifetime.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		/// <returns>A pointer to the single instance.</returns>
		static XentuGame* get_instance(lua_State* L);

		/// Called after the constructor, but before initialize() by c++ 0 for success
		int pre_init(lua_State* L);


#pragma endregion


#pragma region Public Methods


		/// <summary>
		/// Part of the main game loop, draws everything!
		/// </summary>
		void draw(lua_State* L);

		/// <summary>
		/// Set the base path for this game.
		/// </summary>
		void set_base_path(std::string base_path);
		
		/// <summary>
		/// Get the located base path for this game.
		/// </summary>
		std::string get_base_path();

		/// <summary>
		/// Used by the main game loop to see if the game should still be running.
		/// </summary>
		bool is_running();

		/// <summary>
		/// Called when this instance should poll the game window for input.
		/// </summary>
		void poll_input();

		/// <summary>
		/// Set weather or not to use fullscreen.
		/// </summary>
		void set_fullscreen(bool fullscreen);

		/// <summary>
		/// Trigger a named event.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		/// <param name="callbackName">The name of the event to trigger.</param>
		void trigger(lua_State* L, std::string callbackName);

		/// <summary>
		/// Trigger a named event, and pass a single integer parameter.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		/// <param name="callbackName">The name of the event to trigger.</param>
		/// <param name="data">The data to passs with the event.</param>
		void trigger_integer(lua_State* L, std::string callbackName, lua_Integer data);

		/// <summary>
		/// Trigger a named event, and pass a single number (float etc...) parameter.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		/// <param name="callbackName">The name of the event to trigger.</param>
		/// <param name="data">The data to passs with the event.</param>
		void trigger_number(lua_State* L, std::string callbackName, lua_Number data);
		
		/// <summary>
		/// Part of the main game loop, updates any game logic.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		/// <param name="delta">The delta number of seconds since last update.</param>
		void update(lua_State* L, const float delta);


		/// <summary>
		/// Set the currently active scene.
		/// </summary>
		void set_scene(XentuScene* scene);


#pragma endregion


#pragma region Lua Accessible Methods


		/// <summary>
		/// Helper function to allow lua to retrieve the assets property.
		/// </summary>
		int get_assets(lua_State* L);

		/// <summary>
		/// Helper function to allow lua to retrieve the audio property.
		/// </summary>
		int get_audio(lua_State* L);

		/// <summary>
		/// Helper function to allow lua to retrieve the keyboard property.
		/// </summary>
		int get_keyboard(lua_State* L);

		/// <summary>
		/// Helper function to allow lua to retrieve the mouse property.
		/// </summary>
		int get_mouse(lua_State* L);

		/// <summary>
		/// Helper function to allow lua to retrieve the renderer 2D property.
		/// </summary>
		int get_renderer(lua_State* L);

		/// <summary>
		/// Helper function to allow lua to retrieve the viewport property.
		/// </summary>
		int get_viewport(lua_State* L);

		/// <summeray>
		/// Helper function to allow lua to retrieve the config property.
		/// </summary>
		int get_config(lua_State* L);

		/// <summary>
		/// Call this to show a debug report of what is in the lua stack.
		/// </summary>
		int lua_debug_stack(lua_State* L);

		/// <summary>
		/// Called when Lua wishes to close.
		/// </summary>
		int lua_exit(lua_State* L);

		/// <summary>
		/// Allows scripts in lua to subscribe to events that can be fired using the Trigger() method.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		/// <returns>Exit code, 0 unless something went wrong.</returns>
		int lua_on(lua_State* L);

		/// <summary>
		/// The engines version of require() which is sensitive to the data folder.
		/// </summary>
		int lua_require(lua_State* L);		

		/// <summary>
		/// Allows scripts in Lua to trigger events.
		/// </summary>
		int lua_trigger(lua_State* L);

		/// <summary>
		/// Allows scripts in Lua to trigger events with a single argument.
		/// </summary>
		int lua_trigger_with(lua_State* L);

		/// <summary>
		///	Allows the user to set a scene to receive update and draw events. By
		/// default only the game global handles these.
		/// </summary>
		int lua_set_scene(lua_State* L);
		

#pragma endregion


		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<XentuGame>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<XentuGame>::FunctionType methods[];

		AssetManager* assets;
		Configuration* config;
		static bool USE_PROXY_PATH;

		/* Id for our loaded shader. */
		unsigned int shader;

	private:
		/* Here will be the instance stored. */
		static XentuGame* instance;

		/* the base path of the game */
		std::string base_path;

		GLFWwindow* window;
		Renderer2D* renderer;
		AudioPlayer* audio;
		Sprite sprite;
		KeyboardManager* keyboard;
		MouseManager* mouse;
		Viewport* viewport;

		/* random num variable. */
		int num;

		/* true if initialize was called and was successful. */
		bool initialized;

		/* true if the window is closing or a request to close has been made */
		bool m_closing;

		/* A pointer to the currently selected scene, default is nullptr */
		XentuScene* m_current_scene;

		/* Store a hashmap of assigned event callbacks. */
		std::unordered_map<std::string, int> callbacks;
	};
}

#endif