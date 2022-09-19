#pragma once

#include <stdio.h>
#include <string>

#include "XentuConfig.h"
#include "XentuRenderer.h"

namespace xen
{
	class XentuMachine
	{
		public:
			XentuMachine(size_t argc, char *argv[], XentuConfig* config);
			virtual ~XentuMachine();
			// initialize the machine, which runs the initial game script.
			virtual int Init() = 0;
			// triggers a game wide event.
			virtual int Trigger(const string event_name) = 0;
			// called by game code to begin the tight events loop.
			void Run();
			// call this to retrieve the loaded config.
			XentuConfig* GetConfig();
			// call this to retrieve the loaded renderer.
			XentuRenderer* GetRenderer();

		protected:
			const size_t arg_count;
			const char* arg_values[12];

		private:
			XentuConfig* m_config;
			XentuRenderer* m_renderer;
	};
}