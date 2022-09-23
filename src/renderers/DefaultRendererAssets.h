#pragma once

namespace xen
{
	const char * xen_gl_default_vertex_shader = 
	R"(#version 330
		in vec3 i_position;
		in vec2 i_texcoord;
		in vec4 i_color;
		uniform mat4 u_MVP;
		out vec2 v_TexCoord;
		out vec4 v_Color;
		void main()
		{
			gl_Position = u_MVP * vec4(i_position, 1.0); 
			v_TexCoord = i_texcoord;
			v_Color = i_color;
		}
	)";


	const char * xen_gl_default_fragment_shader = 
	R"(#version 330
		//precision mediump float;
		in vec2 v_TexCoord;
		in vec4 v_Color;
		uniform sampler2D u_Texture;
		out vec4 frag_colour;
		void main()
		{
			vec4 texColor = texture(u_Texture, v_TexCoord);
			frag_colour = texColor * v_Color;
		}
	)";
}