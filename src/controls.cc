#include "controls.hh"

#include <glm/gtc/matrix_transform.hpp>

namespace{
float initialFoV = 45.0f * (3.14159f/180.0f);
float speed = 3.0f;
float mouseSpeed = 0.005f;
}

FPS_View::FPS_View(){
	m_position = glm::vec3(-5, 0, 0);
	m_lastTime = 0;
	m_theta = 3.14f/2.0f;
	m_phi = 0;
}

void FPS_View::updateMatrices(NG::Window& window) {
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - m_lastTime);

	if (window.IsFocused() && m_lastfocus) {
		auto cursor = window.GetCursorPos() - window.GetCenter();

		m_phi   -= mouseSpeed * float(cursor.x);
		m_theta += mouseSpeed * float(cursor.y);
		if (m_theta<0) {
			m_theta = 0;
		} else if (m_theta>3.14f) {
			m_theta = 3.14f;
		}
	}

	if (window.IsFocused()){
		window.CenterCursor();
	}


	glm::vec3 direction(sin(m_theta) * cos(m_phi),
	                    sin(m_theta) * sin(m_phi),
	                    cos(m_theta));
	glm::vec3 right(sin(m_phi), -cos(m_phi), 0);
	glm::vec3 up = glm::cross(right, direction);

	glm::vec3 z(0,0,1);

	if (window.IsKeyPressed(GLFW_KEY_W)) {
		m_position += direction * deltaTime * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_S)) {
		m_position -= direction * deltaTime * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_A)) {
		m_position -= right * deltaTime * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_D)) {
		m_position += right * deltaTime * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		m_position += z * deltaTime * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
		m_position -= z * deltaTime * speed;
	}

	float FoV = initialFoV;

	m_proj = glm::perspective(FoV, 4.0f/3.0f, 0.1f, 100.0f);
	m_view = glm::lookAt(m_position, m_position+direction, up);

	m_lastTime = currentTime;
	m_lastfocus = window.IsFocused();
}
