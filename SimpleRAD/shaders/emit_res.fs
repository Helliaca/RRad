#version 450 core

out vec4 FragColor;

in vec2 fsUV;

uniform sampler2D tex2D;

void main()
{
	vec3 col = vec3(0);
	vec2 uv = fsUV;

	float dim = textureSize(tex2D, 0).x;

	uv *= dim;
	uv = mod(uv, 1.0);

	if(uv.x>0.5f ^^ uv.y>0.5f) col = vec3(1.0);

	FragColor = vec4(col, 1.0);
}