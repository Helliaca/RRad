#pragma once


#ifndef ENGINE_H
#define ENGINE_H

#include <mutex>
#include <thread>

#include "../ObjImporter.h"

class Engine
{
private:
	void console();
	std::thread consoleThread;
	std::mutex settingMutex;
public:
	void run();

	Engine();
	~Engine();
};

#endif