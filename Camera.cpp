#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position) {
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Rotation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
	
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform) {
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window) {
	//MOVE
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Position += speed * Rotation;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Position += speed * -Rotation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		Position += speed * -glm::normalize(glm::cross(Rotation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		Position += speed * glm::normalize(glm::cross(Rotation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 0.4f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE){
		speed = 0.1f;
	}

	//Look
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (!firstClick) {

			double mx;
			double my;
			glfwGetCursorPos(window, &mx, &my);

			float rotx = sensitivity * (float)(my - (height / 2)) / height;
			float roty = sensitivity * (float)(mx - (width / 2)) / width;

			glm::vec3 newRotation = glm::rotate(Rotation, glm::radians(-rotx), glm::normalize(glm::cross(Rotation, Up)));


			if (!((glm::angle(newRotation, Up) <= glm::radians(5.0f)) or (glm::angle(newRotation, -Up) <= glm::radians(5.0f)))) {
				Rotation = newRotation;
			}

			Rotation = glm::rotate(Rotation, glm::radians(-roty), Up);
		}
		else {
			firstClick = false;
		}

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}

}