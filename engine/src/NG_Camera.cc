#include "NG_Camera.hh"

NG::Camera::Camera() {
	m_view = glm::mat4(1.0f);
	m_perspective = glm::mat4(1.0f);
}

NG::Camera::Camera(const glm::mat4& view, const glm::mat4& perspective) {
	m_view = view;
	m_perspective = perspective;
}

glm::mat4 NG::Camera::GetView() {
	return m_view;
}

glm::mat4 NG::Camera::GetPerspective() {
	return m_perspective;
}
