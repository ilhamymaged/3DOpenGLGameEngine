#include <Mesh.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
	this->vertices = vertices;
	this->indices = indices;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::renderInstanced(size_t cubePositionsSize)
{
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, cubePositionsSize);
	glBindVertexArray(0);
}

void Mesh::render()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh Mesh::generateBlock()
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	// Cube vertices with normals per face
	vertices = {
		// Front face (normal: +Z)
		{{-0.5f, -0.5f,  0.5f}, {0, 0, 1}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0, 0, 1}, {1.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}, {1.0f, 1.0f}},
		{{-0.5f,  0.5f,  0.5f}, {0, 0, 1}, {0.0f, 1.0f}},

		// Back face (normal: -Z)
		{{-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0, 0, -1}, {0.0f, 1.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0, 0, -1}, {1.0f, 1.0f}},

		// Left face (normal: -X)
		{{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {0.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {-1, 0, 0}, {1.0f, 0.0f}},
		{{-0.5f,  0.5f,  0.5f}, {-1, 0, 0}, {1.0f, 1.0f}},
		{{-0.5f,  0.5f, -0.5f}, {-1, 0, 0}, {0.0f, 1.0f}},

		// Right face (normal: +X)
		{{ 0.5f, -0.5f, -0.5f}, {1, 0, 0}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1, 0, 0}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {1, 0, 0}, {0.0f, 1.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {1, 0, 0}, {1.0f, 1.0f}},

		// Top face (normal: +Y)
		{{-0.5f,  0.5f,  0.5f}, {0, 1, 0}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0, 1, 0}, {1.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0, 1, 0}, {1.0f, 1.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0, 1, 0}, {0.0f, 1.0f}},

		// Bottom face (normal: -Y)
		{{-0.5f, -0.5f,  0.5f}, {0, -1, 0}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0, -1, 0}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0, -1, 0}, {0.0f, 1.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1.0f, 1.0f}}
	};

	indices = {
		// Front
		0, 1, 2, 2, 3, 0,
		// Back
		4, 5, 6, 6, 7, 4,
		// Left
		8, 9, 10, 10, 11, 8,
		// Right
		12, 13, 14, 14, 15, 12,
		// Top
		16, 17, 18, 18, 19, 16,
		// Bottom
		20, 21, 22, 22, 23, 20
	};

	return Mesh(vertices, indices);
}

Mesh Mesh::generateLightBlock()
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	// Cube vertices: positions only — normals and texCoords set to zero
	vertices = {
		// Front face
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

		// Back face
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

		// Left face
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

		// Right face
		{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

		// Top face
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

		// Bottom face
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}
	};

	indices = {
		// Front
		0, 1, 2, 2, 3, 0,
		// Back
		4, 5, 6, 6, 7, 4,
		// Left
		8, 9, 10, 10, 11, 8,
		// Right
		12, 13, 14, 14, 15, 12,
		// Top
		16, 17, 18, 18, 19, 16,
		// Bottom
		20, 21, 22, 22, 23, 20
	};

	return Mesh(vertices, indices);
}

