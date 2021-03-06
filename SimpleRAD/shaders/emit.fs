#version 450 core

out vec4 FragColor;

in vec2 fsUV;

uniform sampler2D tex2D;

void main()
{
	FragColor = texture(tex2D, fsUV);
}