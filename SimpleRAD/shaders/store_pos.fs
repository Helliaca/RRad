#version 450 core

out vec4 FragColor;

in vec3 fsPos;
in vec2 fsUV;

layout(RGBA8) uniform image2D tex2D;

void main()
{
	vec3 pos = (fsPos + 1.0) * 0.5f;
	imageStore(tex2D, ivec2(fsUV*imageSize(tex2D)), vec4(pos, 1.0));
}