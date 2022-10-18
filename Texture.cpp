#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

inline bool fileexists(const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}

Texture::Texture(const char* image, const char* nt, GLenum texType, GLenum slot) {
	type = texType;
	nametype = nt;

	//For some reason it brokey everything. Maybe newer STB uses same texmap default???
	//stbi_set_flip_vertically_on_load(true);
	
	// texure
	int widthImg, heightImg, numColCh;

	unsigned char* bytes;

	if (fileexists(image)) {
		bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
	}
	else {
		bytes = stbi_load("missing.png", &widthImg, &heightImg, &numColCh, 0);
	}

	glGenTextures(1, &ID);
	glActiveTexture(slot);
	unit = slot;
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	if (numColCh == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

	}
	else if (numColCh == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	}
	else if (numColCh == 1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
	}
	else {
		throw std::invalid_argument("Well shit, your image isn't doing too well. That's a you problem :p (col_ch not 4, 3, or 1)");
	}

	glGenerateMipmap(texType);

	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, ID);
}
void Texture::Unbind() {
	glBindTexture(type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}