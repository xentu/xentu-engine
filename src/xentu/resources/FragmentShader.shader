#version 300 es // core for macos, es for everything else

precision mediump float;
out vec4 color;

uniform sampler2D u_Texture;
in vec2 v_TexCoord;
in vec4 v_Color;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor * v_Color;
}