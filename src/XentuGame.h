#ifndef XEN_GAME
#define XEN_GAME

#include <GLFW3/glfw3.h>
#include <unordered_map>
#include <luna/luna.hpp>

#include "audio/AudioPlayer.h"
#include "filesystem/AssetManager.h"
#include "filesystem/Configuration.h"
#include "graphics/Renderer2D.h"
#include "graphics/Quad.h"
#include "graphics/Viewport.h"
#include "input/InputManager.h"

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

        /// <summary>
        /// Gets the single instance available of this class during the application lifetime.
        /// </summary>
        /// <param name="L">Pointer to the running lua_State.</param>
        /// <returns>A pointer to the single instance.</returns>
        static XentuGame* get_instance(lua_State* L);

        /// <summary>
        /// Initializes the game window and OpenGL.
        /// </summary>
        /// <param name="L">Pointer to the running lua_State.</param>
        /// <returns>Exit code, 0 unless something went wrong.</returns>
        int initialize(lua_State* L);

        /// <summary>
        /// Part of the main game loop, updates any game logic.
        /// </summary>
        /// <param name="L">Pointer to the running lua_State.</param>
        void update(lua_State* L);

        /// <summary>
        /// Part of the main game loop, draws everything!
        /// </summary>
        void draw(lua_State* L);

        /// <summary>
        /// Trigger a named event.
        /// </summary>
        /// <param name="L">Pointer to the running lua_State.</param>
        /// <param name="callbackName">The name of the event to trigger.</param>
        void trigger(lua_State* L, std::string callbackName);

        /// <summary>
        /// Allows scripts in lua to subscribe to events that can be fired using the Trigger() method.
        /// </summary>
        /// <param name="L">Pointer to the running lua_State.</param>
        /// <returns>Exit code, 0 unless something went wrong.</returns>
        int on(lua_State* L);

        /// <summary>
        /// Assign a primary texture for use by the loaded shader.
        /// </summary>
        /// <param name="texture">The texture to assign.</param>
        void __use_texture(int texture_id);
        int use_texture(lua_State* L);

        /// <summary>
        /// Exposed to lua to test weather function calls work.
        /// </summary>
        /// <param name="L"></param>
        /// <returns></returns>
        int log(lua_State* L);

        /// <summary>
        /// Used by the main game loop to see if the game should still be running.
        /// </summary>
        bool is_running();

        /// <summary>
        /// Helper function to allow lua to retrieve the audio property.
        /// </summary>
        int get_audio(lua_State* L);

        /// <summary>
        /// Helper function to allow lua to retrieve the assets property.
        /// </summary>
        int get_assets(lua_State* L);

        /// <summary>
        /// Helper function to allow lua to retrieve the renderer 2D property.
        /// </summary>
        int get_renderer(lua_State* L);

        /// <summary>
        /// Helper function to allow lua to retrieve the input property.
        /// </summary>
        int get_input(lua_State* L);

        /// <summary>
        /// Helper function to allow lua to retrieve the viewport property.
        /// </summary>
        int get_viewport(lua_State* L);

        /// <summary>
        /// Called when this instance should poll the game window for input.
        /// </summary>
        void poll_input();

        /// <summary>
        /// Called when Lua wishes to close.
        /// </summary>
        int exit(lua_State* L);

        /// Call this to show a debug report of what is in the lua stack.
        int debug_stack(lua_State* L);

        /// <summary>
        /// Get the current path of the executing binary.
        /// </summary>
        static std::string get_current_directory();


        //Class Constants
        static const char className[];

        // List of class properties that one can set/get from Lua
        static const Luna<XentuGame>::PropertyType properties[];

        // List of class methods to make available in Lua
        static const Luna<XentuGame>::FunctionType methods[];

        AssetManager* assets;
        Configuration* config;

    private:
        /* Here will be the instance stored. */
        static XentuGame* instance;

        GLFWwindow* window;
        Renderer2D* renderer;
        AudioPlayer* audio;
        Sprite sprite;
        Quad quad;
        InputManager* input;
        Viewport* viewport;

        /* random num variable. */
        int num;

        /* true if initialize was called and was successful. */
        bool initialized;

        /* true if the window is closing or a request to close has been made */
        bool m_closing;

        /* Id for our loaded shader. */
        unsigned int shader;

        /* Store a hashmap of assigned event callbacks. */
        std::unordered_map<std::string, int> callbacks;
    };
}

#endif