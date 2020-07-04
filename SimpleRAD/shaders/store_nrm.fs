#version 450 core

out vec4 FragColor;

in vec3 worldPos;
in vec3 ourColor;
in vec2 TexCoord;

layout(RGBA8) uniform image2D tex2D;

void main()
{
	imageStore(tex2D, ivec2(TexCoord*imageSize(tex2D)), vec4(worldPos, 1.0));
}