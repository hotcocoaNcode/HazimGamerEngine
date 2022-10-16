#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//LEFT OFF AT 1:24:31 https://www.youtube.com/watch?v=45MIykWJ-C4

//thanks victor gordon with freeCodeCamp for an entire ass rendering engine tutorial
#include "shaderClass.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

const unsigned int width = 1280;
const unsigned int height = 720;

int main() {

	// if you don't know what this does, go home
	glfwInit();

	// tell glfw what opengl we're using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	// no depracated shit
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//really wonder what this does
	GLFWwindow* window = glfwCreateWindow(width, height, "Hazim Engine", NULL, NULL);
	if (window == NULL) {
		std::cout << "Window creation done fucked up" << std::endl;
		glfwTerminate();
		return -69420;
	}

	//because glfw big dum, gotta tell it to use this window and not some other one
	glfwMakeContextCurrent(window);

	gladLoadGL();

	// set area of doing thing
	glViewport(0, 0, width, height);

	//start up le sader pogam
	Shader shaderProgram("default.vert", "default.frag");

	glm::vec4 lightColor[] = { 
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
	};

	glm::vec3 lightPos[] = { 
		glm::vec3(0.0f, 1.0f, 0.0f)

	};

	int lightType[] = {
		2
	};

	shaderProgram.Activate();
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "totalLights"), (sizeof(lightType) / sizeof(int)));
	std::cout << "sending lights to shader with amount " << std::to_string((sizeof(lightType) / sizeof(int))).c_str() << std::endl;

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	//This is some Yanderedev type shit, but it's broken and 
	//the code only runs once at the start and not at every frame
	//so i honestly do not care.
	//for (int i = 0; i < (sizeof(lightType) / sizeof(int)); ++i) {

		/*
		std::string ic = std::to_string(i);
		const char* lp = ("lightPos" + ic).c_str();
		const char* lc = ("lightColor" + ic).c_str();
		const char* lt = ("lightType" + ic).c_str();
		std::cout << "lightPos iter: " << lp << std::endl;
		*/
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor1"), lightColor[0].x, lightColor[0].y, lightColor[0].z, lightColor[0].w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos1"), lightPos[0].x, lightPos[0].y, lightPos[0].z);
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "lightType1"), lightType[0]);
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor2"), lightColor[1].x, lightColor[1].y, lightColor[1].z, lightColor[1].w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos2"), lightPos[1].x, lightPos[1].y, lightPos[1].z);
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "lightType2"), lightType[1]);
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor3"), lightColor[2].x, lightColor[2].y, lightColor[2].z, lightColor[2].w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos3"), lightPos[2].x, lightPos[2].y, lightPos[2].z);
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "lightType3"), lightType[2]);
	//}
	



	//DRAW BACKROUND
	glClearColor(0.39f, 0.39f, 0.39f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT); //no fucking clue what a buffer bit is but docs said so

	//swap
	glfwSwapBuffers(window);

	glEnable(GL_DEPTH_TEST); // depth so that it doesn't render opposite faces on top

	Camera cam(width, height, glm::vec3(0.0f, 0.5f, 2.5f));

	Model model("sword/scene.gltf");

	while (!glfwWindowShouldClose(window)) {
		// check for shit
		glfwPollEvents();

		// backgrond
		glClearColor(0.39f, 0.39f, 0.39f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //no fucking clue what a buffer bit is but docs said so

		//setup n shit
		cam.Inputs(window);
		cam.updateMatrix(60.0f, 0.1f, 100.0f);

		model.Draw(shaderProgram, cam);

		//swap
		glfwSwapBuffers(window);

	}

	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}