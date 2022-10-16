#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>

#include "shaderClass.h"

class Texture {
	public:
		GLuint ID;
		GLenum type;
		const char* nametype;

		Texture(const char* image, const char* nt, GLenum texType, GLenum slot);

		void texUnit(Shader& shader, const char* uniform, GLuint unit);
		void Bind();
		void Unbind();
		void Delete();
};

#endif