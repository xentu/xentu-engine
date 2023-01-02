#pragma once

#include <stdio.h>
#include <string>

#include "Globals.h"
#include "Config.h"
#include "Renderer.h"
#include "audio/AudioManager.h"
#include "assets/AssetManager.h"
#include "input/InputManager.h"

namespace xen
{
	class Machine
	{
		public:
			Machine(size_t argc, char *argv[], Config* config);
			virtual ~Machine();
			// initialize the machine, which runs the initial game script.
			virtual int Init() = 0;
			// triggers a game wide event.
			virtual int Trigger(const string event_name) = 0;
			// triggers a game wide event with a string arg.
			virtual int Trigger(const string event_name, const string arg0) = 0;
			// triggers a game wide event with an int arg.
			virtual int Trigger(const string event_name, const int arg0) = 0;
			// triggers a game wide event with a float arg.
			virtual int Trigger(const string event_name, const float arg0) = 0;
			// called by game code to begin the tight events loop.
			void Run();
			// call this to retrieve the loaded config.
			Config* GetConfig();
			// call this to retrieve the loaded renderer.
			Renderer* GetRenderer();
			// call this to retrieve the keyboard manager.
			InputManager* GetInput();

		protected:
			const size_t arg_count;
			const char* arg_values[12];

		private:
			Config* m_config;
			AssetManager* m_assets;
			Renderer* m_renderer;
			InputManager* m_input;
	};
}