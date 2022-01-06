#version 410

in vec2 v_TexCoord;
in vec4 v_Color;

uniform sampler2D u_Texture;

out vec4 frag_colour;

void main()
{
    vec4 color = texture(u_Texture, v_TexCoord) * v_Color;
    frag_colour = vec4(color.r, color.r, color.r, color.a);
}