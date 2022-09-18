#ifndef XEN_MACHINE
#define XEN_MACHINE

#include "Xentu.h"
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
			virtual int init();
			// called by game code to begin the tight events loop.
			virtual void run();
			// triggers a game wide event.
			virtual int trigger(const std::string event_name);
			// call this to retrieve the loaded config.
			XentuConfig* get_config();
			// call this to retrieve the loaded renderer.
			XentuRenderer* get_renderer();

		protected:
			const size_t arg_count;
			const char* arg_values[MAX_ARGV];

		private:
			XentuConfig* m_config;
			XentuRenderer* m_renderer;
	};
}

#endif