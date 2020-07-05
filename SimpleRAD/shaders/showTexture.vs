#version 450 core

layout (location = 0) in vec3 vsPos;
layout (location = 1) in vec3 vsColor;
layout (location = 2) in vec3 vsNormal;
layout (location = 3) in vec2 vsUV;

out vec2 fsUV;

void main()
{
	fsUV = vsUV;

	vec2 clipspace = vsUV * 2.0 - 1.0; // Go from [0,1] to [-1,1]
	gl_Position = vec4(clipspace.x, clipspace.y, 0.0, 1.0);
}