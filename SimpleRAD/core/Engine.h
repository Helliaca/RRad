#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "assets/Shader.h"

#ifndef ENGINE_H
#define ENGINE_H

class Engine
{
private:

public:
	void run();

	Engine();
	~Engine();
};

#endif