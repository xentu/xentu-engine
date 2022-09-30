#pragma once

#include <string>
#include <map>
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <GL/glu.h>
#include <lib/glm/glm.hpp>

#include "graphics/TextBox.h"
#include "graphics/Texture.h"
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


	class Renderer
	{
		public:
			Renderer(const Config* config);
			~Renderer();

			/**
			 * Initialize the renderer with default title size, position and mode.
			 */
			bool Init();

			/**
			 * Initialize the renderer.
			 */
			bool Init(string title, int x, int y, int width, int height, int mode, int vp_width, int vp_height, int vp_mode);

			/**
			 * Use a specific shader
			 */
			void UseShader(int asset_id);

			/**
			 * Called at the beginning of each new frame.
			 */
			void NewFrame(bool resized);

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
			 * Draw a rectangle.
			 */
			void DrawRectangle(int x, int y, int width, int height);

			/**
			 * Draw a textbox onto the current buffer.
			 */
			void DrawTextBox(int textbox_id);

			/**
			 * Set the text for a textbox (updates an underlying texture, can be expensive).
			 */
			void SetTextBoxText(int textbox_id, int font_id, const char* text);

			/**
			 * Measure the physical dimensions of text based on textbox and font.
			 */
			Vector2i MeasureTextBoxText(int textbox_id, int font_id, const char* text);

			/**
			 * Set the foreground color of a textbox (updates an underlying texture, can be expensive).
			 */ 
			void SetTextBoxColor(int textbox_id, int font_id, int r, int g, int b);

			/**
			 * Set the color used to clear the buffer.
			 */ 
			void SetClearColor(int r, int g, int b);

			/**
			 * Set the color used to clear the buffer.
			 */ 
			void SetForegroundColor(int r, int g, int b);

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
			int m_shader;
			unsigned int m_shader_transform_loc;
			unsigned int m_shader_tex_loc;

			/* screen/viewport variables */
			Batch* view_batch;
			glm::mat4 view_proj;

			/* screen vars */
			float sc_w; float sc_h;
			glm::mat4 screen_proj;

			/* flags */
			bool m_running;
	};
}