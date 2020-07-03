#include "utils/shader.h"
#include "utils/debug.h"

int main() {
	//Initialize glfw
	glfwInit();

	//This is the OpenGL window we render to. IF the number of literals is increased beyonbd 21, the resolution will have to be appropriately adjusted.
	GLFWwindow* window = glfwCreateWindow(1600, 1600, "SimpleRAD", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	checkErrors("Initialization");

	while (true)
	{

	}

	//Shader solve_shader("core/solve.vs", "core/solve.fs");
}