#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>

#include <glm/mat4x4.hpp> 
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 

class Shader {
public:
	Shader(std::string name);
	~Shader();

	void use();
	void setMat4(glm::mat4& value, const char* name);
	void setVec3(glm::vec3& value, std::string name);
	void setBool(bool value, std::string name);

private:
	GLuint id;

private:
	std::string readConfigFile(std::string filename);

};