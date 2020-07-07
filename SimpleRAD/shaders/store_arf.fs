#version 450 core

out vec4 FragColor;

in float fsA;
in vec2 fsUV;

layout(RGBA8) uniform image2D tex2D;

void main()
{
	imageStore(tex2D, ivec2(fsUV*imageSize(tex2D)), vec4(fsA, fsA, fsA, 1.0));
}