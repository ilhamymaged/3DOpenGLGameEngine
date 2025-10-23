#include <Texture.h>

Texture::Texture(std::string name)
{
	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	std::string temp = "../../../assets/Textures/" + name;
	unsigned char* data = stbi_load(temp.c_str(), &width, &height, &nrChannels, 0);
	if (data != NULL) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "DEBUG::IMAGE HAS BEEN LOADED: " << name << std::endl;
	}
	else {
		std::cerr << "DEBUG::IMAGE HAS NOT BEEN LOADED: "  << name << std::endl;
	}
	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}
