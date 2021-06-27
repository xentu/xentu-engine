#version 320 es

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_MVP;

out vec2 v_TexCoord;

void main()
{
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
}