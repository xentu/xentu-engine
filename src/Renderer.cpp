#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Globals.h"
#include "Config.h"
#include "Renderer.h"
//#include "DefaultRendererAssets.h"


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


	#pragma region Static Utility Methods


	// Compiles an individual vertex or fragment shader.
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
			XEN_ERROR("Failed to compile ", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), " shader\n", message);
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
			XEN_ERROR("Failed to link shader.");
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
			XEN_ERROR(&error[0]);
		}

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}


	#pragma endregion


	DefaultRenderer::DefaultRenderer(const Config* config)
	: m_config(config),
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
		TTF_Init();
		XEN_LOG("- Created DefaultRenderer.\n");
		clear_color_r = 0;
		clear_color_g = 0;
		clear_color_b = 0;
	}


	bool DefaultRenderer::Init()
	{
		return Init(
			m_config->title,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			m_config->window.width,
			m_config->window.height,
			SDL_WINDOW_OPENGL,
			m_config->viewport.width,
			m_config->viewport.height,
			m_config->viewport.mode);
	}


	bool DefaultRenderer::Init(std::string title, int x, int y, int width, int height, int mode, int vp_width, int vp_height, int vp_mode)
	{
		Uint32 render_flags = SDL_RENDERER_ACCELERATED;

		m_viewport = Viewport(vp_width, vp_height, vp_mode);

		//Use OpenGL 3.1 core
      SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
      SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
      SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		#ifdef __APPLE__
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		#endif

		m_window = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_OPENGL);
		//m_renderer = SDL_CreateRenderer(m_window, -1, render_flags);
		m_gl_context = SDL_GL_CreateContext(m_window);

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

			//Use V-Sync
			if (SDL_GL_SetSwapInterval( 1 ) < 0) {
				XEN_WARN( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
			}

			printf("- OpenGL Version: %s\n", glGetString(GL_VERSION));

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
			this->screen_proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f);

			/* get the uniform locations */
			m_shader = create_shader(xen_gl_default_vertex_shader, xen_gl_default_fragment_shader);
			glUseProgram(m_shader);
			m_shader_transform_loc = glGetUniformLocation(m_shader, "u_MVP");
			
			/* set the shader uniforms default */
			glUniformMatrix4fv(m_shader_transform_loc, 1, false, &view_proj[0][0]);

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
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);

			// debug the frame buffer creation.
			auto ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (ret != GL_FRAMEBUFFER_COMPLETE) {
				if (ret == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
					XEN_LOG("Something went wrong with the fbo. GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
				}
				else if (ret == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
					XEN_LOG("Something went wrong with the fbo. GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
				}
				else {
					XEN_LOG("Something went wrong with the fbo. #%s%s%i", ret, ", texture #", m_fbo_texture);
				}
			}
			else {
				printf("Frame buffer created #%i\n", m_fbo_texture);
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
		}
		return true;
	}


	int DefaultRenderer::LoadTexture(uint8_t* buffer, uint64_t length)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto sur = IMG_Load_RW(rw, AUTO_FREE);

		GLuint texture_id = 0;
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		auto mode = GL_RGB;
		if (sur->format->BytesPerPixel == 4) {
			mode = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, mode, sur->w, sur->h, 0, mode, GL_UNSIGNED_BYTE, sur->pixels);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SDL_FreeSurface(sur);

		Texture* texture = new Texture(texture_id, sur->w, sur->h, mode);
		
		m_textures.insert(std::make_pair(m_textures_iter, texture));
		m_textures_iter++;

		return m_textures_iter - 1;
	}


	int DefaultRenderer::LoadFont(uint8_t* buffer, uint64_t length, int font_size)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto font = TTF_OpenFontRW(rw, 1 /* free RWops resource once open */, font_size);

		m_fonts.insert(std::make_pair(m_fonts_iter, font));
		m_fonts_iter++;

		return m_fonts_iter - 1;
	}


	int DefaultRenderer::CreateTextBox(int x, int y, int width, int height)
	{
		auto textbox = new TextBox(x, y, width, height);
		m_textboxes.insert(std::make_pair(m_textboxes_iter, textbox));
		m_textboxes_iter++;
		return m_textboxes_iter - 1;
	}


	bool DefaultRenderer::IsRunning()
	{
		return m_running;
	}


	void DefaultRenderer::Exit()
	{
		m_running = false;
	}


	void DefaultRenderer::NewFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}


	void DefaultRenderer::Begin()
	{
		m_origin_x = 0;
		m_origin_y = 0;
		m_pos_x = 0;
		m_pos_y = 0;
		m_scale_x = 1;
		m_scale_y = 1;
		m_rotation = 0;
	}


	void DefaultRenderer::Clear()
	{
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


	void DefaultRenderer::Present()
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
		int sc_w = m_config->window.width;
		int sc_h = m_config->window.height;
		glViewport(0, 0, sc_w, sc_h);
		glUniformMatrix4fv(m_shader_transform_loc, 1, false, &screen_proj[0][0]);

		// clear the screen once more (this time with black).
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// prepare to draw the render target as a texture.
		m_sprite.ResetTransform();
		m_sprite.ResetTexCoords();
		m_sprite.set_scale(1, 1);

		// choose the viewport mode.
		if (m_viewport.mode == 2) {
			// stretch
			m_sprite.m_width = sc_w;
			m_sprite.m_height = sc_h;
		}
		else if (m_viewport.mode == 1) {
			// centre
			m_sprite.m_width = static_cast<float>(m_fbo_texture_inst->width);
			m_sprite.m_height = static_cast<float>(m_fbo_texture_inst->height);
			const float hx = (sc_w / 2.0) - (vp_w / 2.0);
			const float hy = (sc_h / 2.0) - (vp_h / 2.0);
			m_sprite.set_position(hx, hy);
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
		SDL_Delay(1000 / 60);
	}


	void DefaultRenderer::DrawTexture(int texture_id, int x, int y, int width, int height)
	{
		m_sprite.ResetTransform();
		m_sprite.set_position(m_pos_x + x, m_pos_y + y);
		m_sprite.set_origin(m_origin_x, m_origin_y);
		m_sprite.set_rotation(m_rotation);
		m_sprite.set_scale(m_scale_x, m_scale_y);

		m_sprite.m_color = Vector4f(1, 1, 1, 1); // TODO: fix this.
		m_sprite.m_width = static_cast<float>(width);
		m_sprite.m_height = static_cast<float>(height);
		m_sprite.m_texture = m_textures[texture_id]->gl_texture_id;
		m_sprite.m_rect = Rect(0, 0, 1, 1);

		if (m_sprite.m_texture == NULL)
			return;

		find_batch(m_sprite)->draw(m_sprite);
	}


	void DefaultRenderer::DrawSubTexture(int texture_id, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
	{
		m_sprite.ResetTransform();
		m_sprite.set_position(m_pos_x + x, m_pos_y + y);
		m_sprite.set_origin(m_origin_x, m_origin_y);
		m_sprite.set_rotation(m_rotation);
		m_sprite.set_scale(m_scale_x, m_scale_y);

		m_sprite.m_color = Vector4f(1, 1, 1, 1); // TODO: fix this.
		m_sprite.m_width = static_cast<float>(w);
		m_sprite.m_height = static_cast<float>(h);
		m_sprite.m_texture = m_textures[texture_id]->gl_texture_id;

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


	void DefaultRenderer::DrawTextBox(int textbox_id)
	{
		auto textbox = m_textboxes[textbox_id];

		m_sprite.ResetTransform();
		m_sprite.set_position(m_pos_x + textbox->m_position.x, m_pos_y + textbox->m_position.y);
		m_sprite.set_origin(m_origin_x, m_origin_y);
		m_sprite.set_rotation(m_rotation);
		m_sprite.set_scale(m_scale_x, m_scale_y);

		m_sprite.m_color = Vector4f(1, 1, 1, 1); // TODO: fix this.
		m_sprite.m_width = static_cast<float>(textbox->m_position.w);
		m_sprite.m_height = static_cast<float>(textbox->m_position.h);
		m_sprite.m_texture = textbox->m_texture;
		m_sprite.m_rect = Rect(0, 0, 1, 1);

		if (m_sprite.m_texture == NULL)
			return;

		find_batch(m_sprite)->draw(m_sprite);
	}
	

	void DefaultRenderer::SetTextBoxText(int textbox_id, int font_id, const char* text)
	{
		auto textbox = m_textboxes[textbox_id];
		auto font = m_fonts[font_id];
		textbox->SetText(font, text);
	}

	
	DefaultRenderer::~DefaultRenderer()
	{
		for (auto const& tex : m_textures)
		{
			delete tex.second;
		}
		m_textures.clear();

		for (auto const& tb : m_textboxes)
		{
			delete tb.second;
		}
		m_textures.clear();

		// no need to delete fonts.
		m_fonts.clear();
		TTF_Quit();
		SDL_DestroyWindow(m_window);
		XEN_LOG("- Destroyed DefaultRenderer\n");
	}


	void DefaultRenderer::SetClearColor(int r, int g, int b)
	{
		clear_color_r = static_cast<GLclampf>(r) / 255.0f;
		clear_color_g = static_cast<GLclampf>(g) / 255.0f;
		clear_color_b = static_cast<GLclampf>(b) / 255.0f;
	}


	void DefaultRenderer::SetWindowMode(XenWindowMode mode)
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


	Batch* DefaultRenderer::find_batch(const Sprite& sprite)
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
	

	void DefaultRenderer::sort_batches()
	{
		// Sort the batches
		std::sort(m_batches.begin(), m_batches.end(), [&](const Batch* a_batch, const Batch* b_batch)
			{
				if (a_batch->m_layer == b_batch->m_layer)
					return a_batch->m_texture < b_batch->m_texture;

				return false; //a_batch->m_layer < a_batch->m_layer;
			});
	}
}