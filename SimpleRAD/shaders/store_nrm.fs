#version 450 core

out vec4 FragColor;

in vec3 fsNormal;
in vec2 fsUV;

layout(RGBA8) uniform image2D tex2D;

void main()
{
	imageStore(tex2D, ivec2(fsUV*imageSize(tex2D)), vec4(fsNormal, 1.0));
}