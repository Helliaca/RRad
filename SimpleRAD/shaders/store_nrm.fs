#version 450 core

out vec4 FragColor;

in vec3 fsNormal;
in vec2 fsUV;

layout(RGBA8) uniform image2D tex2D;

void main()
{
	vec3 n = fsNormal;
	n = normalize(fsNormal);
	n = (n + 1.0) * 0.5f; //[-1, 1] -> [0, 1]
	imageStore(tex2D, ivec2(fsUV*imageSize(tex2D)), vec4(n, 1.0));
}