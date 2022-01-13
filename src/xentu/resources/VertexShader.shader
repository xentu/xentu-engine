#version 330

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