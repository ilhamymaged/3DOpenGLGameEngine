#pragma once
#include <glad/glad.h>

#include <iostream>
#include <string>

#include <stb_image.h>

class Texture {
public:
	Texture(std::string name);
	~Texture();

	void bind();
private:
	unsigned int id;
};