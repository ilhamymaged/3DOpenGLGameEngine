#pragma once
#include <glm/mat4x4.hpp> 
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 

class Camera {
public:
	Camera(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float fov = 45.0f, float yaw = -90.0f,
		float pitch = 0.0f);

	inline glm::vec3& getPos() { return pos; }
	inline glm::vec3& getFront() { return front; }
	inline glm::vec3& getUp() { return up; }
	inline float& getFOV() { return fov; }
	inline float& getYaw() { return yaw; }
	inline float& getPitch() { return pitch; }

	inline void setPos(glm::vec3 pos) { this->pos = pos; }
	inline void setFront(glm::vec3 front) { this->front = front; }
	inline void setUp(glm::vec3 up) { this->up = up; }
	inline void setYaw(float yaw) { this->yaw = yaw; }
	inline void setPitch(float pitch) { this->pitch = pitch; }
	inline void setFOV(float fov) { this->fov = fov; }

	glm::mat4 getViewMat();

private:
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;

	float fov;
	float yaw;
	float pitch;
};

