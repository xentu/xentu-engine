#pragma once

#define MAX_WINDOW_COUNT 1
#define AUTO_FREE 1

#include <string>
#include <map>
#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <gl/glu.h>
#include <lib/glm/glm.hpp>

#include "graphics/TextBox.h"
#include "graphics/Batch.h"

using namespace std;

namespace xen
{
	class DefaultRenderer
	{
		public:
			DefaultRenderer(const Config* config);
			~DefaultRenderer();

			/**
			 * Initialize the renderer with default title size, position and mode.
			 */
			bool Init();

			/**
			 * Initialize the renderer.
			 */
			bool InitEx(string title, int x, int y, int width, int height, int mode);

			/**
			 * Load a texture into memory, and return it's asset id.
			 */
			int LoadTexture(uint8_t* buffer, uint64_t length);

			/**
			 * Load a ttf font into memory, and return it's asset id.
			 */
			int LoadFont(uint8_t* buffer, uint64_t length, int font_size);

			/**
			 * Create a textbox with specific dimensions, and return it's asset id.
			 */
			int CreateTextBox(int x, int y, int width, int height);

			/**
			 * Check to see if the renderers window/context is still running.
			 */
			bool IsRunning();

			/**
			 * Called at the beginning of each new frame.
			 */
			void NewFrame();

			/**
			 * Called by a machine to begin a new rendering batch. Begin should
			 * not be called again until Present is called. Multiple Begin/Present
			 * pairs in one draw call is counted as a batch.
			 */
			void Begin();

			/**
			 * Clears the current draw buffer.
			 */
			void Clear();

			/**
			 * Present the current draw buffer to the screen.
			 */
			void Present();

			/**
			 * Tell the underlying window/context to close.
			 */
			void Exit();

			/**
			 * Draw a texture onto the current buffer.
			 */
			void DrawTexture(int texture_id, int x, int y, int width, int height);
			
			/**
			 * Draw part of a texture onto the current buffer.
			 */
			void DrawSubTexture(int texture_id, int x, int y, int w, int h, int sx, int sy, int sw, int sh);

			/**
			 * Draw a textbox onto the current buffer.
			 */
			void DrawTextBox(int textbox_id);

			/**
			 * Set the text for a textbox (updates an underlying texture, can be expensive).
			 */
			void SetTextBoxText(int textbox_id, int font_id, const char* text);

			/**
			 * Check to see if a keyboard key is down.
			 */
			bool KeyDown(int key_code);

			/**
			 * Set the color used to clear the buffer.
			 */ 
			void SetClearColor(int r, int g, int b);


		private:
			/**
			 * Find the correct batch to use for rendering a sprite.
			 */
			Batch* find_batch(const Sprite& sprite);


		private:
			const Config* m_config;
			GLclampf clear_color_r, clear_color_g, clear_color_b;

			/* sdl variables */
			SDL_Window* m_window;
			SDL_Renderer* m_renderer;
			SDL_GLContext m_gl_context = NULL;

			/* state information */
			std::vector<Batch*> m_batches; // the collection of batches.
			Sprite m_sprite;  // the one sprite instance we modify per draw.
			float m_rotation; // the chosen rotational angle for the next sprite.
			float m_pos_x;
			float m_pos_y;
			float m_origin_x; // the chosen x origin for the next sprite.
			float m_origin_y; // the chosen y origin for the next sprite.
			float m_scale_x; // the chosen x scale factor for the next sprite.
			float m_scale_y; // the chosen y scale factor for the next sprite.

			/* the vertex/index/frame buffer. */
			unsigned int m_vao;
			unsigned int m_vbo;
			unsigned int m_ibo;
			unsigned int m_fbo;
			unsigned int m_fbo_texture;
			Texture* m_fbo_texture_inst;
			Texture* white_texture;

			/* shader information. */
			unsigned int m_shader;
			unsigned int m_shader_transform_loc;
			unsigned int m_shader_tex_loc;

			/* textures */
			map<int, Texture*> m_textures;
			int m_textures_iter = 0;

			/* fonts */
			map<int, TTF_Font*> m_fonts;
			int m_fonts_iter = 0;

			/* textboxes */
			map<int, TextBox*> m_textboxes;
			int m_textboxes_iter = 0;

			/* keyboard event buffer */
			SDL_Keysym m_keyboard_events[40];
			int m_keyboard_events_iter = 0;

			/* screen/viewport variables */
			Batch* view_batch;
			glm::mat4 view_proj;
			int vp_mode;

			/* flags */
			bool m_exiting = false;
	};
}