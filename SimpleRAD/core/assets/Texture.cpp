#include "Texture.h"


Texture::Texture() {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Set wrapping options
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//LOD and Texture settings
	//smooth version:
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Mipmaps
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Texture
	//non-smooth version
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); //Mipmaps
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //Texture

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, MAP_RES, MAP_RES); //Specify storage

	clear();
}

void Texture::clear() {
	glBindTexture(GL_TEXTURE_2D, textureID);

	const std::vector<GLubyte> texBuffer(4 * MAP_RES * MAP_RES, 0); //3 because RGB

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, MAP_RES, MAP_RES, GL_RGBA, GL_UNSIGNED_BYTE, &texBuffer[0]); //Set colors for lowest mipmap level (all black)

	glBindTexture(GL_TEXTURE_2D, 0); //Unbind Texture
}

void Texture::use(const int shaderProgramID, const std::string glSamplerName, const int textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(shaderProgramID, glSamplerName.c_str()), textureUnit);
}
