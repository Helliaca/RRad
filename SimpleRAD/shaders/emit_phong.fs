#version 450 core

out vec4 FragColor;

in vec3 fsPos;
in vec3 fsColor;
in vec3 fsNormal;
in vec2 fsUV;

uniform sampler2D tex2D;

void main()
{
	vec3 lightPos = vec3(0.0, 0.95, -0.35);
	vec3 lightColor = vec3(1.0f);

	// Ambient
	vec3 ambient = fsColor * lightColor * 0.05f;
  	
	// Diffuse
	vec3 lightDir = normalize(lightPos - fsPos);
	float diff = max(dot(-fsNormal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor * 1.0f;

	// Attenuation
	float distance = length(lightPos - fsPos);
	float attenuation = 1.0f / (0.1f + 0.1f * distance + 0.5f * (distance * distance));
	vec3 col = (ambient + diffuse) * fsColor * attenuation;

	FragColor = vec4(col, 1.0f);
}