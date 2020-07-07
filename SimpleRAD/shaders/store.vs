#version 450 core

layout (location = 0) in vec3 vsPos;
layout (location = 1) in vec3 vsColor;
layout (location = 2) in vec3 vsNormal;
layout (location = 3) in vec2 vsUV;
layout (location = 4) in float vsA;

out vec3 fsPos;
out vec3 fsColor;
out vec3 fsNormal;
out vec2 fsUV;
out float fsA;

void main()
{
	fsPos = vsPos;
	fsColor = vsColor;
	fsNormal = vsNormal;
	fsUV = vsUV;
	fsA = vsA;

	vec2 clipspace = vsUV * 2.0 - 1.0; // Go from [0,1] to [-1,1]
	gl_Position = vec4(clipspace.x, clipspace.y, 0.0, 1.0);
}