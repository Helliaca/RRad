#version 450 core

layout (location = 0) in vec3 vsPos;
layout (location = 1) in vec3 vsColor;
layout (location = 2) in vec3 vsNormal;
layout (location = 3) in vec2 vsUV;

out vec3 fsPos;
out vec3 fsColor;
out vec3 fsNormal;
out vec2 fsUV;

void main()
{
	mat4 m = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	mat4 v = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0,-1.0, 0.0,
		0.0, 0.0,-2.5, 1.0
	);

	// Taken from Übung 3:
	mat4 p = mat4(
		3.7, 0.0, 0.0, 0.0,
		0.0, 6.6, 0.0, 0.0,
		0.0, 0.0,-1.6,-6.6,
		0.0, 0.0,-1.0, 0.0
	);

	gl_Position = p * v * m * vec4(vsPos, 1.0);
	fsColor = vsColor;
	fsUV = vsUV;
	fsPos = vsPos;
	fsNormal = vsNormal;
}