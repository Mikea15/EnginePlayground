#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D TexAlbedo;

void main()
{
    FragColor = vec4(texture(TexAlbedo, TexCoords).rgb, 1.0f);
}
