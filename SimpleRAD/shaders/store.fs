#version 450 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

layout(RGBA8) uniform image2D tex2D;

void main()
{
	FragColor = vec4(1.0);
	//FragColor = texture(texture1, TexCoord);

	// store ourColor into texture1 at position TexCoord
	imageStore(tex2D, ivec2(TexCoord*imageSize(tex2D)), vec4(1.0));
}