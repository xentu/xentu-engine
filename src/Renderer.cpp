#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Globals.h"
#include "Config.h"
#include "Renderer.h"
#include "assets/AssetManager.h"
//#include "RendererAssets.h"


// macro for calculating the byte offset of vertex information.
#define BUFFER_OFFSET(i) ((float*)NULL + (i))


namespace xen
{
	#pragma region Default Shader Test


	const char * xen_gl_default_vertex_shader = 
	R"(#version 330
		in vec3 i_position;
		in vec2 i_texcoord;
		in vec4 i_color;
		uniform mat4 u_MVP;
		out vec2 v_TexCoord;
		out vec4 v_Color;
		void main()
		{
			gl_Position = u_MVP * vec4(i_position, 1.0); 
			v_TexCoord = i_texcoord;
			v_Color = i_color;
		}
	)";


	const char * xen_gl_default_fragment_shader = 
	R"(#version 330
		//precision mediump float;
		in vec2 v_TexCoord;
		in vec4 v_Color;
		uniform sampler2D u_Texture;
		out vec4 frag_colour;
		void main()
		{
			vec4 texColor = texture(u_Texture, v_TexCoord);
			frag_colour = texColor * v_Color;
		}
	)";


	#pragma endregion


	Renderer::Renderer(const Config* config)
	:  m_config(config),
		m_viewport(640, 480, 2),
		m_rotation(0),
		m_origin_x(0),
		m_origin_y(0),
		m_pos_x(0),
		m_pos_y(0),
		m_scale_x(1),
		m_scale_y(1),
		m_ibo(-1),
		m_vbo(-1),
		m_sprite(),
		m_running(true)
	{
		clear_color_r = 0;
		clear_color_g = 0;
		clear_color_b = 0;
	}


	void Renderer::UseShader(int asset_id)
	{
		unsigned int shader_id = AssetManager::GetInstance()->GetShader(asset_id);
		glUseProgram(shader_id);
		m_shader_transform_loc = glGetUniformLocation(shader_id, "u_MVP");	
		/* set the shader uniforms default */
		glUniformMatrix4fv(m_shader_transform_loc, 1, false, &view_proj[0][0]);
	}


	unsigned int Renderer::GetUniformLocation(string name)
	{
		unsigned int shader_id = AssetManager::GetInstance()->GetShader(m_shader_asset_id);
		return glGetUniformLocation(shader_id, name.c_str());
	}


	bool Renderer::Init()
	{
		return Init(
			m_config->title,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			m_config->window.width,
			m_config->window.height,
			SDL_WINDOW_OPENGL,
			m_config->resizable,
			m_config->viewport.width,
			m_config->viewport.height,
			m_config->viewport.mode);
	}


	bool Renderer::Init(std::string title, int x, int y, int width, int height, int mode, bool resizable, int vp_width, int vp_height, int vp_mode)
	{
		m_viewport = Viewport(vp_width, vp_height, vp_mode);

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        	std::cout << "SDL could not be initialized: " << SDL_GetError();
    	}
		else
		{
        	std::cout << "SDL video system is ready to go\n";
    	}
	
		//Use OpenGL 3.3 core
		if (SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 ) < 0) {
			std::cout << "SDL could not set OpenGL Major Version: " << SDL_GetError();
		}

      	if (SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 ) < 0) {
			std::cout << "SDL could not set OpenGL Minor Version: " << SDL_GetError();
		}

		//r = SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
      	if (SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE ) < 0) {
			std::cout << "SDL could not set OpenGL Core Profile: " << SDL_GetError();
		}

		if (resizable) {
			m_window = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		}
		else {
			m_window = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_OPENGL);
		}
		
		m_gl_context = SDL_GL_CreateContext(m_window);

		//Use V-Sync
		if (m_config->v_sync) {
			if (SDL_GL_SetSwapInterval(1) < 0) {
				XEN_ERROR("> Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			}
			else {
				std::cout << "> V-Sync Enabled" << std::endl;
			}
		}
		else {
			if (SDL_GL_SetSwapInterval(0) < 0) {
				XEN_ERROR("> Warning: Unable to set swap interval to immediate! SDL Error: %s\n", SDL_GetError());
			}
			else {
				std::cout << "> V-Sync Disabled" << std::endl;
			}
		}

		if(m_gl_context == NULL) {
			XEN_ERROR("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}
		else {
			//Initialize GLEW
			glewExperimental = GL_TRUE;
			GLenum glewError = glewInit();
			if(glewError != GLEW_OK)
			{
				XEN_ERROR( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
				return false;
			}

			XEN_ECHO("> OpenGL Version: %s\n", glGetString(GL_VERSION));

			// generate a white (10x10) texture.
			unsigned char* wt_data = new unsigned char[400];
			for (int i = 0; i < 400; i++) wt_data[i] = 255;
			unsigned int wt_ref;
			glGenTextures(1, &wt_ref);
			glBindTexture(GL_TEXTURE_2D, wt_ref);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 10, 10, 0, GL_RGBA, GL_UNSIGNED_BYTE, wt_data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			white_texture = new Texture(wt_ref, 10, 10, 4);

			/* calculate the projection matrices */
			glViewport(0, 0, m_viewport.width, m_viewport.height);
			this->view_proj = glm::ortho(0.0f, (float)m_viewport.width, (float)m_viewport.height, 0.0f);
			sc_w = (float)width;
			sc_h = (float)height;
			this->screen_proj = glm::ortho(0.0f, sc_w, sc_h, 0.0f);

			/* load default shader, and begin using it. */
			m_shader_asset_id = AssetManager::GetInstance()->LoadShader(xen_gl_default_vertex_shader, xen_gl_default_fragment_shader);
			UseShader(m_shader_asset_id);

			/* generate a vertex array object */
			glGenVertexArrays(1, &m_vao);

			// prepare vertex buffer.
			glGenBuffers(1, &m_vbo);
			glBindVertexArray(m_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

			// Specify the vertex layout (pos + uv + color).
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, BUFFER_OFFSET(0));
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, BUFFER_OFFSET(3));
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, BUFFER_OFFSET(5));

			// Make sure the attributes are marked as enabled.
			glEnable(GL_DEPTH_TEST);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			// prepare frame buffer.
			glGenFramebuffers(1, &m_fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

			// prepare frame buffer texture.
			glGenTextures(1, &m_fbo_texture);
			glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_viewport.width, m_viewport.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);

			// debug the frame buffer creation.
			auto ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (ret != GL_FRAMEBUFFER_COMPLETE) {
				if (ret == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
					XEN_ERROR("> Something went wrong with the fbo. GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
				}
				else if (ret == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
					XEN_ERROR("> Something went wrong with the fbo. GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
				}
				else {
					XEN_ERROR("> Something went wrong with the fbo. #%s%s%i", ret, ", texture #", m_fbo_texture);
				}
			}
			else {
				XEN_ECHO("> Frame buffer created #%i\n", m_fbo_texture);
			}

			// create a texture for our generated fbo.
			m_fbo_texture_inst = new Texture(m_fbo_texture, m_viewport.width, m_viewport.height, 4);
			view_batch = new Batch();
			view_batch->m_texture = m_fbo_texture_inst->gl_texture_id;

			// unbind the frame buffer for now.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// create an index buffer.
			glGenBuffers(1, &m_ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
			
			// set some basic render modes.
			glDisable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// see if we need to set a window icon.
			if (m_config->icon.length() > 0) {
				SDL_Surface* icon = AssetManager::GetInstance()->LoadTextureSurface(m_config->icon);
				SDL_SetWindowIcon(m_window, icon);
				SDL_FreeSurface(icon);
			}
		}

		drawn_screen = Rect(0, 0, (float)vp_width, (float)vp_height);

		return true;
	}


	bool Renderer::IsRunning()
	{
		return m_running;
	}


	void Renderer::Exit()
	{
		m_running = false;
	}


	void Renderer::NewFrame(bool resized)
	{
		if (resized) {
			int width = 800;
			int height = 600;
			SDL_GetWindowSize(m_window, &width, &height);
			sc_w = (float)width;
			sc_h = (float)height;
			this->screen_proj = glm::ortho(0.0f, sc_w, sc_h, 0.0f);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void Renderer::Begin(bool reset_transforms)
	{
		if (reset_transforms) {
			m_origin_x = 0;
			m_origin_y = 0;
			m_pos_x = 0;
			m_pos_y = 0;
			m_scale_x = 1;
			m_scale_y = 1;
			m_rotation = 0;
			m_alpha = 1;
		}
	}


	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		for (unsigned int i = 0; i < m_batches.size(); i++)
		{
			m_batches[i]->clear();

			if (m_batches[i]->m_inactive > 256)
			{
				std::swap(m_batches[i], m_batches.back());
				delete m_batches.back();
				m_batches.pop_back();
				i--;
			}
		}
	}


	void Renderer::Present()
	{
		const size_t vertex_size = sizeof(Vertex) * 4; // 4 vertices per quad.
		const size_t element_size = sizeof(unsigned int) * 6; // 6 indices per quad.
		int vp_w = m_viewport.width;
		int vp_h = m_viewport.height;

		// assign the frame buffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		// setup the camera.
		glViewport(0, 0, vp_w, vp_h);
		glUniformMatrix4fv(m_shader_transform_loc, 1, false, &view_proj[0][0]);

		// clear the screen.
		glClearColor(clear_color_r, clear_color_g, clear_color_b, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw each of the batches (layers)
		for (const Batch* batch : m_batches)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, batch->m_texture);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, batch->quad_count * vertex_size, batch->m_vertices.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch->quad_count * element_size, batch->m_indices.data(), GL_DYNAMIC_DRAW);

			glDrawElements(GL_TRIANGLES, batch->quad_count * 6, GL_UNSIGNED_INT, nullptr);
		}

		// unbind the frame buffer.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, (int)sc_w, (int)sc_h);
		glUniformMatrix4fv(m_shader_transform_loc, 1, false, &screen_proj[0][0]);

		// clear the screen once more (this time with black).
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// prepare to draw the render target as a texture.
		m_sprite.ResetTransform();
		m_sprite.ResetTexCoords();
		m_sprite.set_scale(1, 1);

		// choose the viewport mode.
		drawn_screen.left = 0;
		drawn_screen.top = 0;
		if (m_viewport.mode == 2) {
			// stretch
			m_sprite.m_width = static_cast<float>(sc_w);
			m_sprite.m_height = static_cast<float>(sc_h);
		}
		else if (m_viewport.mode == 1) {
			// centre
			m_sprite.m_width = static_cast<float>(m_fbo_texture_inst->width);
			m_sprite.m_height = static_cast<float>(m_fbo_texture_inst->height);
			const float hx = (sc_w / 2.0) - (vp_w / 2.0);
			const float hy = (sc_h / 2.0) - (vp_h / 2.0);
			m_sprite.set_position(hx, hy);
			drawn_screen.left = static_cast<float>(hx);
			drawn_screen.top = static_cast<float>(hy);
		}
		else {
			// none
			m_sprite.m_width = static_cast<float>(m_fbo_texture_inst->width);
			m_sprite.m_height = static_cast<float>(m_fbo_texture_inst->height);
		}
		
		
		m_sprite.m_texture = m_fbo_texture_inst->gl_texture_id;
		m_sprite.m_color = Vector4f(1, 1, 1, 1);

		// prepare the view_batch and draw it.
		view_batch->clear();
		view_batch->draw_inverted(m_sprite);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, view_batch->quad_count * vertex_size, view_batch->m_vertices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, view_batch->quad_count * element_size, view_batch->m_indices.data(), GL_DYNAMIC_DRAW);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		//SDL_RenderPresent(m_renderer);
		SDL_GL_SwapWindow(m_window);

		// only impose delay if one is set.
		if (m_config->draw_frequency > 0 && m_config->draw_frequency < 999) {
			SDL_Delay(1000 / m_config->draw_frequency);
		}
	}


	void Renderer::DrawTexture(int texture_id, int x, int y, int width, int height)
	{
		m_sprite.ResetTransform();
		m_sprite.set_position(m_pos_x + x, m_pos_y + y);
		m_sprite.set_origin(m_origin_x, m_origin_y);
		m_sprite.set_rotation(m_rotation);
		m_sprite.set_scale(m_scale_x, m_scale_y);

		m_sprite.m_color = Vector4f(1, 1, 1, m_alpha); // TODO: fix this.
		m_sprite.m_width = static_cast<float>(width);
		m_sprite.m_height = static_cast<float>(height);
		m_sprite.m_texture = AssetManager::GetInstance()->GetTexture(texture_id)->gl_texture_id;
		m_sprite.m_rect = Rect(0, 0, 1, 1);

		if (m_sprite.m_texture == NULL)
			return;

		find_batch(m_sprite)->draw(m_sprite);
	}


	void Renderer::DrawSubTexture(int texture_id, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
	{
		m_sprite.ResetTransform();
		m_sprite.set_position(m_pos_x + x, m_pos_y + y);
		m_sprite.set_origin(m_origin_x, m_origin_y);
		m_sprite.set_rotation(m_rotation);
		m_sprite.set_scale(m_scale_x, m_scale_y);

		m_sprite.m_color = Vector4f(1, 1, 1, m_alpha); // TODO: fix this.
		m_sprite.m_width = static_cast<float>(w);
		m_sprite.m_height = static_cast<float>(h);
		m_sprite.m_texture = AssetManager::GetInstance()->GetTexture(texture_id)->gl_texture_id;

		// calculate the sub-rect.
		float sx_p = (float)sx / m_sprite.m_width;
		float sy_p = (float)sy / m_sprite.m_height;
		float sw_p = (float)sw / m_sprite.m_width;
		float sh_p = (float)sh / m_sprite.m_height;
		m_sprite.m_rect = Rect(sx_p, sy_p, sw_p, sh_p);

		if (m_sprite.m_texture == NULL)
			return;

		find_batch(m_sprite)->draw(m_sprite);
	}


	void Renderer::DrawRectangle(int x, int y, int width, int height)
	{
		m_sprite.ResetTransform();
		m_sprite.set_position(m_pos_x + x, m_pos_y + y);
		m_sprite.set_origin(m_origin_x, m_origin_y);
		m_sprite.set_rotation(m_rotation);
		m_sprite.set_scale(m_scale_x, m_scale_y);
		
		m_sprite.m_width = (float)width;
		m_sprite.m_height = (float)height;
		m_sprite.m_texture = white_texture->gl_texture_id;
		m_sprite.m_rect = Rect(0, 0, 10, 10);
		m_sprite.m_color = m_fore_color_v;
		find_batch(m_sprite)->draw(m_sprite);
	}


	void Renderer::DrawTextBox(int textbox_id, int x, int y)
	{
		auto textbox = AssetManager::GetInstance()->GetTextBox(textbox_id);

		m_sprite.ResetTransform();
		m_sprite.set_position(m_pos_x + x, m_pos_y + y);
		m_sprite.set_origin(m_origin_x, m_origin_y);
		m_sprite.set_rotation(m_rotation);
		m_sprite.set_scale(m_scale_x, m_scale_y);

		m_sprite.m_color = Vector4f(1, 1, 1, m_alpha); // TODO: fix this.
		m_sprite.m_width = static_cast<float>(textbox->m_position.w);
		m_sprite.m_height = static_cast<float>(textbox->m_position.h);
		m_sprite.m_texture = textbox->m_texture;
		m_sprite.m_rect = Rect(0, 0, 1, 1);

		if (m_sprite.m_texture == NULL)
			return;

		find_batch(m_sprite)->draw(m_sprite);
	}


	void Renderer::DrawSprite(int sprite_map_id, string const& group_name, int frame, int x, int y, int w, int h)
	{
		auto assets = AssetManager::GetInstance();
		auto sprite_map = assets->GetSpriteMap(sprite_map_id);
		auto sprite_group = sprite_map->get_group(group_name);
		auto f = sprite_group->get_frame(frame);
		auto r = f->coords;
		
		int texture_asset_id = sprite_map->get_texture();

		int _x = static_cast<int>(m_pos_x) + x;
		int _y = static_cast<int>(m_pos_y) + y;
		int _scale_x = 1;
		int _scale_y = 1;

		// todo: move flipping of sprites into the shaders.

		if (f->flip_x) {
			_x += w;
			_scale_x = -1;
		}

		if (f->flip_y) {
			_y += h;
			_scale_y = -1;
		}

		m_sprite.ResetTransform();
		m_sprite.set_origin(m_origin_x, m_origin_y);
		m_sprite.set_rotation(m_rotation);
		m_sprite.set_scale(static_cast<float>(_scale_x), static_cast<float>(_scale_y));
		m_sprite.set_position(static_cast<float>(_x), static_cast<float>(_y));

		m_sprite.m_color = Vector4f(1, 1, 1, m_alpha); // TODO: fix this.
		m_sprite.m_width = static_cast<float>(w);
		m_sprite.m_height = static_cast<float>(h);
		m_sprite.m_texture = assets->GetTexture(texture_asset_id)->gl_texture_id;
		m_sprite.m_rect = Rect(r->left, r->top, r->width, r->height);
		
		if (m_sprite.m_texture == NULL)
			return;

		find_batch(m_sprite)->draw(m_sprite);
	}
	

	void Renderer::DrawTileLayer(int tilemap_id, int layer_index)
	{
		auto assets = AssetManager::GetInstance();
		auto tilemap = assets->GetTileMap(tilemap_id);
		auto layer = tilemap->GetLayer(layer_index);

		for (int i=0; i<layer->m_tile_count; i++)
		{
			const auto& tile = layer->m_tiles[i];
			if (tile->texture_id < 0) continue;

			m_sprite.ResetTransform();
			m_sprite.set_position(m_pos_x + tile->x, m_pos_y + tile->y);
			m_sprite.set_scale(1, 1);
			m_sprite.set_rotation(0);
			//m_sprite.m_color = color; //TODO: stack member duplication.

			auto texture = assets->GetTexture(tile->texture_id);

			m_sprite.m_width = static_cast<float>(tile->width);
			m_sprite.m_height = static_cast<float>(tile->height);
			m_sprite.m_texture = texture->gl_texture_id;

			float u = tile->t_x / (float)texture->width;
			float v = tile->t_y / (float)texture->height;
			float w = tile->t_width / (float)texture->width;
			float h = tile->t_height / (float)texture->height;

			//m_sprite.m_rect = Rect(u, 1.0 - v - h,w,h);
			m_sprite.m_rect = Rect(u, v, w, h);
			find_batch(m_sprite)->draw(m_sprite);
		}

		const int texture_id = layer->GetTextureID();
		if (texture_id > 0) {
			m_sprite.ResetTransform();
			m_sprite.set_position(m_pos_x, m_pos_y);
			m_sprite.set_scale(1, 1);
			m_sprite.set_rotation(0);
			m_sprite.m_color = xen::Vector4f(1, 1, 1, 1); //TODO: stack member duplication.
			m_sprite.m_width = 2048;
			m_sprite.m_height = 2048;
			m_sprite.m_texture = assets->GetTexture(texture_id)->gl_texture_id;
			m_sprite.m_rect = Rect(0, 0, 1, 1);
			find_batch(m_sprite)->draw(m_sprite);
		}

		const int obj_count = layer->GetObjectCount();
		if (obj_count > 0) {
			for (int i=0; i<obj_count; i++) {
 				auto obj = layer->GetObject(i);
				if (obj->has_tile) {
					const auto tile = obj->GetTile();
					
					if (tile.texture_id <= 0) continue;
					m_sprite.ResetTransform();
					m_sprite.set_position(m_pos_x + obj->x, m_pos_y + obj->y);
					m_sprite.set_scale(1, 1);
					m_sprite.set_rotation(0);
					m_sprite.m_width = static_cast<float>(tile.width);
					m_sprite.m_height = static_cast<float>(tile.height);

					auto texture = assets->GetTexture(tile.texture_id);
					m_sprite.m_texture = texture->gl_texture_id;

					float u = tile.t_x / (float)texture->width;
					float v = tile.t_y / (float)texture->height;
					float w = tile.t_width / (float)texture->width;
					float h = tile.t_height / (float)texture->height;

					m_sprite.m_rect = Rect(u, 1.0 - v - h,w,h);
					find_batch(m_sprite)->draw(m_sprite);
				}
			}
		}
	}


	void Renderer::SetTextBoxText(int textbox_id, int font_id, const char* text)
	{
		auto assets = AssetManager::GetInstance();
		auto textbox = assets->GetTextBox(textbox_id);
		auto font = assets->GetFont(font_id);
		textbox->SetText(font, text);
	}


	Vector2i Renderer::MeasureTextBoxText(int textbox_id, int font_id, const char* text)
	{
		auto assets = AssetManager::GetInstance();
		auto textbox = assets->GetTextBox(textbox_id);
		auto font = assets->GetFont(font_id);
		return textbox->MeasureText(font, text);
	}


	void Renderer::SetTextBoxColor(int textbox_id, int font_id, int r, int g, int b)
	{
		auto assets = AssetManager::GetInstance();
		auto textbox = assets->GetTextBox(textbox_id);
		auto font = assets->GetFont(font_id);
		SDL_Color color;
		color.r = static_cast<unsigned int>(r);
		color.g = static_cast<unsigned int>(g);
		color.b = static_cast<unsigned int>(b);
		textbox->SetColor(font, color);
	}

	
	Renderer::~Renderer()
	{
		SDL_DestroyWindow(m_window);
	}


	void Renderer::SetClearColor(int r, int g, int b)
	{
		clear_color_r = static_cast<GLclampf>(r) / 255.0f;
		clear_color_g = static_cast<GLclampf>(g) / 255.0f;
		clear_color_b = static_cast<GLclampf>(b) / 255.0f;
	}

	
	void Renderer::SetForegroundColor(int r, int g, int b)
	{
		m_fore_color.r = static_cast<unsigned int>(r);
		m_fore_color.g = static_cast<unsigned int>(g);
		m_fore_color.b = static_cast<unsigned int>(b);
		m_fore_color_v = Vector4f(
			static_cast<GLclampf>(r) / 255.0f,
			static_cast<GLclampf>(g) / 255.0f,
			static_cast<GLclampf>(g) / 255.0f,
			1.0f
		);
	}


	void Renderer::SetWindowMode(XenWindowMode mode)
	{
		int gl_mode = 0;
		if (mode == XenWindowMode::Fullscreen) {
			gl_mode = SDL_WINDOW_FULLSCREEN;
		}
		else if (mode == XenWindowMode::FullscreenWindowed) {
			gl_mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		SDL_SetWindowFullscreen(m_window, gl_mode);
	}


	void Renderer::SetPosition(float x, float y)
	{
		m_pos_x = x;
		m_pos_y = y;
	}


	void Renderer::SetOrigin(float x, float y)
	{
		m_origin_x = x;
		m_origin_y = y;
	}


	void Renderer::SetRotation(float angle)
	{
		m_rotation = angle;
	}


	void Renderer::SetScale(float x, float y)
	{
		m_scale_x = x;
		m_scale_y = y;
	}


	Batch* Renderer::find_batch(const Sprite& sprite)
	{
		for (Batch* batch : m_batches)
		{
			if (batch->m_texture == sprite.m_texture && batch->m_layer == sprite.m_layer)
				return batch;
		}

		Batch* batch = new Batch();
		batch->m_texture = sprite.m_texture;
		batch->m_layer = sprite.m_layer;
		batch->m_inactive = 0;
		batch->quad_count = 0;

		m_batches.push_back(batch);
		return m_batches.back();
	}
	

	void Renderer::sort_batches()
	{
		// Sort the batches
		std::sort(m_batches.begin(), m_batches.end(), [&](const Batch* a_batch, const Batch* b_batch)
			{
				if (a_batch->m_layer == b_batch->m_layer)
					return a_batch->m_texture < b_batch->m_texture;

				return false; //a_batch->m_layer < a_batch->m_layer;
			});
	}


	void Renderer::SetAlpha(float alpha)
	{
		m_alpha = alpha;
		m_sprite.m_color.w = m_alpha;
	}
	

	void Renderer::SetUniforms(int uniform_id, int argc, bool argv[])
	{
		if (argc == 1) glUniform1i(uniform_id, argv[0]);
		else if (argc == 2) glUniform2i(uniform_id, argv[0], argv[1]);
		else if (argc == 3) glUniform3i(uniform_id, argv[0], argv[1], argv[2]);
		else if (argc == 4) glUniform4i(uniform_id, argv[0], argv[1], argv[2], argv[3]);
	}

	
	void Renderer::SetUniforms(int uniform_id, int argc, int argv[])
	{
		if (argc == 1) glUniform1i(uniform_id, argv[0]);
		else if (argc == 2) glUniform2i(uniform_id, argv[0], argv[1]);
		else if (argc == 3) glUniform3i(uniform_id, argv[0], argv[1], argv[2]);
		else if (argc == 4) glUniform4i(uniform_id, argv[0], argv[1], argv[2], argv[3]);
	}
	
	
	void Renderer::SetUniforms(int uniform_id, int argc, float argv[])
	{
		if (argc == 1) glUniform1f(uniform_id, argv[0]);
		else if (argc == 2) glUniform2f(uniform_id, argv[0], argv[1]);
		else if (argc == 3) glUniform3f(uniform_id, argv[0], argv[1], argv[2]);
		else if (argc == 4) glUniform4f(uniform_id, argv[0], argv[1], argv[2], argv[3]);
	}
	
	
	void Renderer::SetUniformMatrix(int uniform_id, glm::mat4 matrix)
	{
		glUniformMatrix4fv(uniform_id, 1, false, &matrix[0][0]);
	}


	void Renderer::SetBlend(const bool enable)
	{
		if (enable) {
			glEnable(GL_BLEND);
		}
		else {
			glDisable(GL_BLEND);
		}
	}


	void Renderer::SetBlendFunc(const int s_factor, const int d_factor)
	{
		glBlendFunc(s_factor, d_factor);
	}


	void Renderer::SetBlendPreset(const BlendPreset preset, const bool p_alpha)
	{
		glEnable(GL_BLEND);
		switch (preset) {
			case BLEND_SOURCE_OVER:
				if (p_alpha) {
					glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    				glBlendEquationSeparate(GL_ADD, GL_ADD);
				}
				else {
					glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    				glBlendEquationSeparate(GL_ADD, GL_ADD);
				}
				break;
			case BLEND_SOURCE_IN:
				// TODO: finish this.
				break;
			case BLEND_SOURCE_OUT:
				// TODO: finish this.
				break;
			case BLEND_SOURCE_ATOP:
				// TODO: finish this.
				break;
			case BLEND_DESTINATION_OVER:
				// TODO: finish this.
				break;
			case BLEND_DESTINATION_IN:
				// TODO: finish this.
				break;
			case BLEND_DESTINATION_OUT:
				// TODO: finish this.
				break;
			case BLEND_DESTINATION_ATOP:
				// TODO: finish this.
				break;
			case BLEND_LIGHTER:
				// TODO: finish this.
				break;
			case BLEND_COPY:
				// TODO: finish this.
				break;
			case BLEND_XOR:
				// TODO: finish this.
				break;
			case BLEND_MULTIPLY:
				// TODO: finish this.
				break;
			case BLEND_SCREEN:
				// TODO: finish this.
				break;
			case BLEND_OVERLAY:
				// TODO: finish this.
				break;
			case BLEND_DARKEN:
				// TODO: finish this.
				break;
			case BLEND_LIGHTEN:
				// TODO: finish this.
				break;
			case BLEND_COLOR_DODGE:
				// TODO: finish this.
				break;
			case BLEND_COLOR_BURN:
				// TODO: finish this.
				break;
			case BLEND_HARD_LIGHT:
				// TODO: finish this.
				break;
			case BLEND_SOFT_LIGHT:
				// TODO: finish this.
				break;
			case BLEND_DIFFERENCE:
				// TODO: finish this.
				break;
			case BLEND_EXCLUSION:
				// TODO: finish this.
				break;
			case BLEND_HUE:
				// TODO: finish this.
				break;
			case BLEND_SATURATION:
				// TODO: finish this.
				break;
			case BLEND_COLOR:
				// TODO: finish this.
				break;
			case BLEND_LUMINOSITY:
				// TODO: finish this.
				break;
		};


		
	}


	const SDL_Color& Renderer::GetForeColor()
	{
		return m_fore_color;
	}
}