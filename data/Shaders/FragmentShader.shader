#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
in vec2 v_TexCoord;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
    //vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Texture, v_TexCoord).r);
    //color = vec4(1.0, 1.0, 1.0, 1.0) * sampled;
}