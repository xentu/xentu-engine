#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <GL/glew.h>

namespace xen {

	inline const GLchar* xen_test_vertex_shader =
	R"(
		attribute vec4 position;
		uniform float aspect;
		varying vec3 color;
		void main()
		{
			gl_Position = vec4(position.xyz, 1.0);
			gl_Position.y *= aspect;
			color = gl_Position.xyz + vec3(0.5);
		}
	)";


	inline const GLchar* xen_test_fragment_shader =
	R"(
		precision mediump float;
	    varying vec3 color;
	    void main()
	    {
	        gl_FragColor = vec4(color, 1.0);
	    }
	)";


	inline const GLchar* xen_gles_default_vertex_shader =
	R"(
		attribute vec3 i_position;
		attribute vec2 i_texcoord;
		attribute vec4 i_color;
		uniform mat4 u_MVP;
		varying vec2 v_TexCoord;
		varying vec4 v_Color;
		void main()
		{
			gl_Position = u_MVP * vec4(i_position, 1.0);
			v_TexCoord = i_texcoord;
			v_Color = i_color;
		}
	)";


	inline const GLchar* xen_gles_default_fragment_shader =
	R"(
		precision mediump float;
		varying vec2 v_TexCoord;
		varying vec4 v_Color;
		uniform sampler2D u_Texture;
		void main()
		{
			vec4 texColor = texture2D(u_Texture, v_TexCoord);
			gl_FragColor = texColor * v_Color;
		}
	)";

}

#endif //CONSTANTS_H