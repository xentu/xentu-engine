#version 410

in vec2 position;
in vec2 texCoord;
in vec4 color;

uniform mat4 u_MVP;

out vec2 v_TexCoord;
out vec4 v_Color;

void main()
{
   gl_Position = u_MVP * vec4(position, 1, 1);
   v_TexCoord = texCoord;
   v_Color = color;
}