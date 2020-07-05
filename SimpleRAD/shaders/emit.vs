#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

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
		0.0, 0.0,-2.0, 1.0
	);

	// Taken from Übung 3:
	mat4 p = mat4(
		3.7, 0.0, 0.0, 0.0,
		0.0, 6.6, 0.0, 0.0,
		0.0, 0.0,-1.6,-6.6,
		0.0, 0.0,-1.0, 0.0
	);

	gl_Position = p * v * m * vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}