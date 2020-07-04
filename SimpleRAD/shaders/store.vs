#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	vec2 clipspace = aTexCoord * 2.0 - 1.0; // Go from [0,1] to [-1,1]
	gl_Position = vec4(clipspace.x, clipspace.y, 0.0, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}