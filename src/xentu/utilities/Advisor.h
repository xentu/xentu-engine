#ifndef XEN_ADVISOR_HPP
#define XEN_ADVISOR_HPP
#define GLFW_INCLUDE_NONE


#include <GLEW/GL/glew.h>
#include <GLFW3/glfw3.h>
#include <chrono>
#include <iostream>
#include <string>

#include "Helper.h"


using namespace std::chrono_literals;


namespace xen
{
	

	/// <summary>
	/// A monolithic utility for reporting/advising things to the console/user.
	/// </summary>
	class Advisor
	{
	public:
		static char const* glErrorString(GLenum const err) noexcept
		{
			switch (err)
			{
				// opengl 2 errors (8)
				case GL_NO_ERROR:
					return "GL_NO_ERROR";

				case GL_INVALID_ENUM:
					return "GL_INVALID_ENUM";

				case GL_INVALID_VALUE:
					return "GL_INVALID_VALUE";

				case GL_INVALID_OPERATION:
					return "GL_INVALID_OPERATION";

				case GL_STACK_OVERFLOW:
					return "GL_STACK_OVERFLOW";

				case GL_STACK_UNDERFLOW:
					return "GL_STACK_UNDERFLOW";

				case GL_OUT_OF_MEMORY:
					return "GL_OUT_OF_MEMORY";

				case GL_TABLE_TOO_LARGE:
					return "GL_TABLE_TOO_LARGE";
					// opengl 3 errors (1)

				case GL_INVALID_FRAMEBUFFER_OPERATION:
					return "GL_INVALID_FRAMEBUFFER_OPERATION";
					// gles 2, 3 and gl 4 error are handled by the switch above

				default:
					throwError("Unknown Error!");
					return nullptr;
			}
		}


		static void throwOnGLError(const char* stage)
		{
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				if (m_show_errors) {
					std::cout << "ERR: A rendering error occured when '" << stage << "': " << Advisor::glErrorString(err) << " " << std::endl;
				}
				
			}
		}


		template <typename ... Args >
		static void throwError(Args ... args)
		{
			if (m_show_errors) {
				std::stringstream ss;
				std::initializer_list<int> unused{ (ss << args, 0)... };
				if (m_show_date) {
					std::cout << now() << " - ERR: An unexpected error occured: " << ss.str() << " " << std::endl;
				}
				else {
					std::cout << " - ERR: An unexpected error occured: " << ss.str() << " " << std::endl;
				}
			}
		}


		template <typename ... Args >
		static void throwWarning(Args ... args)
		{
			if (m_show_warnings) {
				std::stringstream ss;
				std::initializer_list<int> unused{ (ss << args, 0)... };
				if (m_show_date) {
					std::cout << now() << " - WARN: An unexpected error occured: " << ss.str() << " " << std::endl;
				}
				else {
					std::cout << " - WARN: An unexpected error occured: " << ss.str() << " " << std::endl;
				}
			}
		}


		template <typename ... Args >
		static void logInfo(Args ... args)
		{
			if (m_show_info) {
				std::stringstream ss;
				std::initializer_list<int> unused{ (ss << args, 0)... };
				if (m_show_date) {
					std::cout << now() << " - " << ss.str() << " " << std::endl;
				}
				else {
					std::cout << " - " << ss.str() << " " << std::endl;
				}
				
			}
		}


		/// <summary>
		/// Sets the advisory mode switches.
		/// </summary>
		/// <param name="errors">Enable error messages.</param>
		/// <param name="warnings">Enable warning messages.</param>
		/// <param name="advice">Enable developer advice.</param>
		/// <param name="info">Enable debug info.</param>
		static void setMode(bool errors, bool warnings, bool advice, bool info);

		/// <summary>
		/// Set weather to include the date and time before each log entry.
		/// </summary>
		static void setModeDate(bool date);


	private:
		static bool m_show_errors; // errors that should not be ignored.
		static bool m_show_warnings; // warnings that can be ignored.
		static bool m_show_advice; // advice to the end user (game designer).
		static bool m_show_info; // debug information (should be off during production).
		static bool m_show_date; // show or hide date prefixes.


		static const std::string now() {
			time_t now = time(0);
			struct tm  tstruct;
			char buf[80];
			tstruct = *localtime(&now);
			strftime(buf, sizeof(buf), "%y-%m-%d.%X", &tstruct);
			return buf;
		}
	};
}

#endif