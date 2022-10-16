#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


Texture::Texture(const char* image, const char* nt, GLenum texType, GLenum slot) {
	type = texType;
	nametype = nt;
	//useful asfuck
	stbi_set_flip_vertically_on_load(1);
	// texure
	int widthImg, heightImg, numColCh;
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &ID);
	glActiveTexture(slot);
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
		throw std::invalid_argument("Well shit, your image isn't doing too well. That's a you problem :p");
	}

	glGenerateMipmap(texType);

	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
	GLuint tex0Uni = glGetUniformLocation(shader.ID, "tex0");
	shader.Activate();
	glUniform1i(tex0Uni, 0);
}

void Texture::Bind() {
	glBindTexture(type, ID);
}

void Texture::Unbind() {
	glBindTexture(type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}