#version 330

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