#include "../Xentu.h"
#include "../XentuConfig.h"
#include "../XentuRenderer.h"
#include "XentuSDLTextBox.h"
#include "XentuSDLRenderer.h"
#include "XentuSDLRendererAssets.h"

namespace xen
{
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


	XentuSDLRenderer::XentuSDLRenderer(const XentuConfig* config)
	:	XentuRenderer::XentuRenderer(config)
	{
		TTF_Init();
		XEN_LOG("- Created XentuSDLRenderer.\n");
	}


	bool XentuSDLRenderer::Init()
	{
		return InitEx(
			m_config->title,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			m_config->window.width,
			m_config->window.height,
			SDL_WINDOW_OPENGL);
	}


	bool XentuSDLRenderer::InitEx(std::string title, int x, int y, int width, int height, int mode)
	{
		Uint32 render_flags = SDL_RENDERER_ACCELERATED;

		//Use OpenGL 3.1 core
      SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
      SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
      SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		#ifdef __APPLE__
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		#endif

		m_window = SDL_CreateWindow(title.c_str(), x, y, width, height, mode);
		m_renderer = SDL_CreateRenderer(m_window, -1, render_flags);
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

			m_shader = create_shader(xen_gl_default_vertex_shader, xen_gl_default_fragment_shader);
			glUseProgram(m_shader);
			//Initialize OpenGL
			/* if(!initGL()) {
				XEN_ERROR( "Unable to initialize OpenGL!\n" );
				return false;
			} */
		}
		return true;
	}


	int XentuSDLRenderer::LoadTexture(uint8_t* buffer, uint64_t length)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto sur = IMG_Load_RW(rw, AUTO_FREE);
		auto tex = SDL_CreateTextureFromSurface(m_renderer, sur);
		SDL_FreeSurface(sur);
		
		m_textures.insert(std::make_pair(m_textures_iter, tex));
		m_textures_iter++;

		return m_textures_iter - 1;
	}


	int XentuSDLRenderer::LoadFont(uint8_t* buffer, uint64_t length, int font_size)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto font = TTF_OpenFontRW(rw, 1 /* free RWops resource once open */, font_size);

		m_fonts.insert(std::make_pair(m_fonts_iter, font));
		m_fonts_iter++;

		return m_fonts_iter - 1;
	}


	int XentuSDLRenderer::CreateTextBox(int x, int y, int width, int height)
	{
		auto textbox = new XentuSDLTextBox(m_renderer, x, y, width, height);
		m_textboxes.insert(std::make_pair(m_textboxes_iter, textbox));
		m_textboxes_iter++;
		return m_textboxes_iter - 1;
	}


	bool XentuSDLRenderer::IsRunning()
	{
		bool running = true;
		SDL_Event event;

		m_keyboard_events_iter = 0;
		
		// Events management
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					// handling of close button
					running = false;
					break;
				case SDL_KEYDOWN:
					m_keyboard_events[m_keyboard_events_iter] = event.key.keysym;
					m_keyboard_events_iter++;
					break;
      	}
		}

		return running && !m_exiting;
	}


	void XentuSDLRenderer::Exit()
	{
		m_exiting = true;
	}


	bool XentuSDLRenderer::KeyDown(int key_code)
	{
		for (int i=0; i<m_keyboard_events_iter; i++) {
			auto evt = m_keyboard_events[i];
			if (evt.scancode == key_code) return true;
		}
		return false;
	}


	void XentuSDLRenderer::Prepare()
	{
		SDL_SetRenderDrawColor(m_renderer, clear_color_r, clear_color_g, clear_color_b, 255);
		SDL_RenderClear(m_renderer);
	}


	void XentuSDLRenderer::Present()
	{
		SDL_RenderPresent(m_renderer);
		SDL_Delay(1000 / 60);
	}


	void XentuSDLRenderer::DrawTexture(int texture_id, int x, int y, int width, int height)
	{
		SDL_Texture* tex = m_textures[texture_id];
		SDL_Rect dest;
		dest.x = x;
		dest.y = y;
		dest.w = width;
		dest.h = height;
		SDL_RenderCopy(m_renderer, tex, NULL, &dest);
	}


	void XentuSDLRenderer::DrawSubTexture(int texture_id, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
	{
		SDL_Texture* tex = m_textures[texture_id];
		SDL_Rect dest;
		dest.x = x;
		dest.y = y;
		dest.w = w;
		dest.h = h;
		SDL_Rect src;
		src.x = sx;
		src.y = sy;
		src.w = sw;
		src.h = sh;
		SDL_RenderCopy(m_renderer, tex, &src, &dest);
	}


	void XentuSDLRenderer::DrawTextBox(int textbox_id)
	{
		auto textbox = m_textboxes[textbox_id];
		SDL_Texture* tex = textbox->texture;
		SDL_RenderCopy(m_renderer, tex, NULL, &textbox->position);
	}
	

	void XentuSDLRenderer::SetTextBoxText(int textbox_id, int font_id, const char* text)
	{
		auto textbox = m_textboxes[textbox_id];
		auto font = m_fonts[font_id];
		textbox->SetText(font, text);
	}

	
	XentuSDLRenderer::~XentuSDLRenderer()
	{
		for (auto const& tex : m_textures)
		{
			SDL_DestroyTexture(tex.second);
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
		XEN_LOG("- Destroyed XentuSDLRenderer\n");
	}
}