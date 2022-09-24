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
#include "graphics/Viewport.h"

using namespace std;

namespace xen
{
	enum XenWindowMode
	{
		Windowed = 0,
		Fullscreen = 1,
		FullscreenWindowed = 2
	};


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
			bool Init(string title, int x, int y, int width, int height, int mode, int vp_width, int vp_height, int vp_mode);

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
			 * Called at the beginning of each new frame.
			 */
			void NewFrame();

			/**
			 * Begin a new batch of graphics to draw, also resets the global transform.
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
			 * Check if the renderer is running.
			 */
			bool IsRunning();

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
			 * Set the color used to clear the buffer.
			 */ 
			void SetClearColor(int r, int g, int b);

			/**
			 * Set the window mode.
			 */ 
			void SetWindowMode(XenWindowMode mode);

			/**
			 * Set the global transform translation.
			 */
			void SetPosition(float x, float y);

			/**
			 * Set the global transform origin.
			 */
			void SetOrigin(float x, float y);

			/**
			 * Set the global transform rotation.
			 */
			void SetRotation(float angle);

			/**
			 * Set the global transform scale.
			 */
			void SetScale(float x, float y);


		private:
			/**
			 * Find the correct batch to use for rendering a sprite.
			 */
			Batch* find_batch(const Sprite& sprite);

			/**
			 * Sort the buffered batches by layer order.
			 */
			void sort_batches();


		private:
			const Config* m_config;
			Viewport m_viewport;
			GLclampf clear_color_r, clear_color_g, clear_color_b;

			/* sdl variables */
			SDL_Window* m_window;
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

			/* screen/viewport variables */
			Batch* view_batch;
			glm::mat4 view_proj;
			glm::mat4 screen_proj;

			/* flags */
			bool m_running;
	};
}