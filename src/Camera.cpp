#include <Camera.h>

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov, float yaw, float pitch)
{
	this->pos = pos;
	this->front = front;
	this->up = up;
	this->fov = fov;
	this->yaw = yaw;
	this->pitch = pitch;
}

glm::mat4 Camera::getViewMat()
{
	return glm::lookAt(pos, pos + front, up);
}
