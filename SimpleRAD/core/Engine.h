#pragma once


#ifndef ENGINE_H
#define ENGINE_H

#include <mutex>
#include <thread>

#include "../ObjImporter.h"

#include "assets/Texture.h"

#include "assets/Shader.h"

class Engine
{
private:
	void console();
	std::thread consoleThread;
	std::mutex settingMutex;

	unsigned int VBO, VAO, EBO;
	unsigned int VAO_size;

	void make_Tex(Texture* tex, std::string fragShader);
	void make_ligTex(Texture* tex, Texture* posTex, Texture* nrmTex, Texture* arfTex, Texture* old_ligTex, int pass);

	GLFWwindow* window;

	void load_world();
public:
	void run();

	Engine();
	~Engine();
};

#endif