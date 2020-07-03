#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

class Texture
{
public:
	//ID
	unsigned int textureID;

	Texture();
	~Texture();

	//Methods
	void clear();
	void use(const int shaderProgramID, const std::string glSamplerName, const int textureUnit);
};

