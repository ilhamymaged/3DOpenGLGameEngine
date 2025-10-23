#pragma once
#include <iostream>
#include <vector>
#include <Shader.h>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class Mesh {
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();
	void renderInstanced(size_t);
	void render();

	static Mesh generateBlock();
	static Mesh generateLightBlock();

	inline GLuint getVAO() const { return VAO; }

private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GLuint VBO, VAO, EBO;
};