#pragma once
#include <Camera.h>
#include <GLFW/glfw3.h>

class InputHandler {
public:
	InputHandler(Camera& camera);

	void processInput(GLFWwindow* window);
	void mouse_callback_impl(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset);

	inline static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
		if (handler)
			handler->mouse_callback_impl(window, xpos, ypos);
	}

	inline static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
		if (handler)
			handler->scroll_callback_impl(window, xoffset, yoffset);
	}

	inline bool isPaused() const { return paused; }
	inline void setDeltaTime(float dt) { deltaTime = dt; }

private:
	Camera& camera;
	bool paused;
	bool ctrlPressedLastFrame;
	bool firstMouse;
	float deltaTime;
	float lastX;
	float lastY;

};