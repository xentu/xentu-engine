#pragma once

#include <string>
#include "Config.h"

using namespace std;


namespace xen
{
	class XentuRenderer
	{
		public:
			XentuRenderer(const XentuConfig* config);
			virtual ~XentuRenderer();
			virtual bool Init() = 0;
			virtual bool InitEx(string title, int x, int y, int width, int height, int mode) = 0;
			virtual int LoadTexture(uint8_t* buffer, uint64_t length) = 0;
			virtual int LoadFont(uint8_t* buffer, uint64_t length, int font_size) = 0;
			virtual int CreateTextBox(int x, int y, int width, int height) = 0;
			virtual bool IsRunning() = 0;
			virtual void Prepare() = 0;
			virtual void Present() = 0;
			virtual void Exit() = 0;
			virtual void DrawTexture(int texture_id, int x, int y, int width, int height) = 0;
			virtual void DrawSubTexture(int texture_id, int x, int y, int w, int h, int sx, int sy, int sw, int sh) = 0;
			virtual void DrawTextBox(int textbox_id) = 0;
			virtual void SetTextBoxText(int textbox_id, int font_id, const char* text) = 0;
			virtual bool KeyDown(int key_code) = 0;
			void SetClearColor(int r, int g, int b);

		protected:
			const XentuConfig* m_config;
			int clear_color_r, clear_color_g, clear_color_b;
	};
}