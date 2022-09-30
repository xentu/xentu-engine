#pragma once

#include <iostream>
#include <exception>
#include <string.h>
#include <stdlib.h>

using namespace std;

namespace xen
{
	static char* strdup_ex(const char* s)
	{
		size_t slen = strlen(s);
		char* result = (char *)malloc(slen + 1);
		if(result == NULL)
		{
			return NULL;
		}

		memcpy(result, s, slen+1);
		return result;
	}


	struct XentuNotFoundException : public exception {
		private:
			const char* m_file;
		public:
			XentuNotFoundException(const char* filename)
			{
				// use strdup to make a copy of the filename as it might get deleted
				// before the exception is handled.
				m_file = strdup_ex(filename);
			}
			const char * what() { return m_file; }
	};
}