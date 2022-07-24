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
			XentuMachine(size_t argc, char *argv[], const XentuConfig* config);
			~XentuMachine();
			virtual int run(const std::string entry_point);
			virtual int trigger(const std::string event_name);
			XentuRenderer* get_renderer();

		protected:
			const size_t arg_count;
			const char* arg_values[MAX_ARGV];

		private:
			const XentuConfig* m_config;
			XentuRenderer* m_renderer;
	};
}

#endif