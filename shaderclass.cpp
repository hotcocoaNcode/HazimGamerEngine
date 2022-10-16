#include "shaderClass.h"
#include <iostream>


inline bool fileexists(const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}

std::string get_file_contents(const char* filename)
{
	if (fileexists(filename)) {
		std::ifstream in(filename, std::ios::binary);
		std::string content((std::istreambuf_iterator<char>(in)),
			(std::istreambuf_iterator<char>()));
		return content;
	}
	else {
		std::string failvar = "uhohwhoashit";
		std::cout << "File \"" << filename << "\" not found... huge L. Returning with " << failvar << std::endl;
		return failvar;
	}
	
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	std::cout << "Loaded Vertex Shader Source (" << vertexFile << ")" << std::endl;
	const char* fragmentSource = fragmentCode.c_str();
	std::cout << "Loaded Fragment Shader Source (" << fragmentFile << ")" << std::endl;

	// shader shit, shader shit, does whatever a shader shits
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// can it swing from a web
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	// no but it can cause a segfault
	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	//not needed after linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR on " << type << std::endl;
			std::cout << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR on " << type << std::endl;
			std::cout << infoLog << std::endl;
		}
	}
}