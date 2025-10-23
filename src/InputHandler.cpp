#include <InputHandler.h>

InputHandler::InputHandler(Camera& camera): paused{false}, ctrlPressedLastFrame{false}, firstMouse{true},
lastX{ 1280 / 2.0f }, lastY{ 720 / 2.0f }, camera{ camera }, deltaTime{ 0.0f }
{
}

void InputHandler::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (!paused) {
        float scalar = deltaTime * 2.5f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.getPos() += camera.getFront() * scalar;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.getPos() -= camera.getFront() * scalar;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.getPos() += 
			glm::normalize(glm::cross(camera.getFront(), camera.getUp())) * scalar;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.getPos() -= 
			glm::normalize(glm::cross(camera.getFront(), camera.getUp())) * scalar;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.getPos() += camera.getUp() * scalar;
    }

    bool ctrlPressed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
    if (ctrlPressed && !ctrlPressedLastFrame) {
        paused = !paused;
        glfwSetInputMode(window, GLFW_CURSOR, paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
    ctrlPressedLastFrame = ctrlPressed;
}

void InputHandler::mouse_callback_impl(GLFWwindow* window, double xpos, double ypos)
{
	if (paused) return;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	float yaw = camera.getYaw();
	float pitch = camera.getPitch();

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	camera.setYaw(yaw);
	camera.setPitch(pitch);

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera.setFront(glm::normalize(direction));
}

void InputHandler::scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.setFOV(camera.getFOV() - (float)yoffset * 5.0f);
	if (camera.getFOV() < 1.0f)
		camera.setFOV(1.0f);
	if (camera.getFOV() > 45.0f)
		camera.setFOV(45.0f);
}
