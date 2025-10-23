#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class WindowManager {
public:
	WindowManager(const char* title, int widht, int height);
	~WindowManager();

	inline void swapBuffers() { glfwSwapBuffers(window); }
	inline void pullEvents() { glfwPollEvents(); }

	inline GLFWwindow* getWindow() { return window; }
private:
	GLFWwindow* window;
};

