#ifndef XEN_EXCEPTIONS
#define XEN_EXCEPTIONS

#include <iostream>
#include <exception>

using namespace std;

namespace xen
{
	struct XentuNotFoundException : public exception {
		private:
			const char* m_file;
		public:
			XentuNotFoundException(const char* filename)
			{
				// use strdup to make a copy of the filename as it might get deleted
				// before the exception is handled.
				m_file = strdup(filename);
			}
			const char * what() { return m_file; }
	};
}

#endif