#include "controls.hh"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 viewMatrix;
glm::mat4 projMatrix;

glm::mat4 getViewMatrix() {
	return viewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return projMatrix;
}

glm::vec3 position = glm::vec3(-5, 0, 0);
float theta = 3.14f/2.0f;
float phi = 0.0f;
float initialFoV = 45.0f * (3.14159f/180.0f);

float speed = 3.0f;
float mouseSpeed = 0.005f;

void computeMatricesFromInputs(NG::Window& window) {
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	if (window.IsFocused()) {
		auto cursor = window.GetCursorPos() - window.GetCenter();
		window.CenterCursor();

		phi   -= mouseSpeed * float(cursor.x);
		theta += mouseSpeed * float(cursor.y);
		if (theta<0) {
			theta = 0;
		} else if (theta>3.14f) {
			theta = 3.14f;
		}
	}

	glm::vec3 direction(sin(theta) * cos(phi),
	                    sin(theta) * sin(phi),
	                    cos(theta));
	glm::vec3 right(sin(phi), -cos(phi), 0);
	glm::vec3 up = glm::cross(right, direction);

	glm::vec3 z(0,0,1);

	if (window.IsKeyPressed(GLFW_KEY_W)) {
		position += direction * deltaTime * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_S)) {
		position -= direction * deltaTime * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_A)) {
		position -= right * deltaTime * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_D)) {
		position += right * deltaTime * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		position += z * deltaTime * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
		position -= z * deltaTime * speed;
	}

	float FoV = initialFoV;

	projMatrix = glm::perspective(FoV, 4.0f/3.0f, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(position, position+direction, up);

	lastTime = currentTime;
}
