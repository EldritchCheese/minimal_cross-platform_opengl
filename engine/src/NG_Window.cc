#include "NG_Window.hh"

#include <stdexcept>
#include <map>

namespace {
std::weak_ptr<NG::GLFW> s_glfw;
std::weak_ptr<NG::GLEW> s_glew;

std::map<GLFWwindow*, NG::Window* > s_window_map;

void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	NG::Window* ng_window;
	try {
		ng_window = s_window_map.at(window);
	} catch (std::out_of_range e) {
		return;
	}
	ng_window->ScrollCallback(xoffset,yoffset);
}

void glfw_focus_callback(GLFWwindow* window, int focused) {
	NG::Window* ng_window;
	try {
		ng_window = s_window_map.at(window);
	} catch (std::out_of_range e) {
		return;
	}
	ng_window->FocusCallback(focused);
}
}

NG::Window::Window(int width, int height, const char* name, GLFWmonitor* monitor, GLFWwindow* share) {
	if (!s_glfw.lock()) {
		s_glfw = m_glfw = std::make_shared<NG::GLFW>();
	}

	m_width = width;
	m_height = height;
	m_name = name;
	m_window = glfwCreateWindow(m_width, m_height, m_name, monitor, share);
	if (m_window==NULL) {
		throw std::runtime_error("Could not make glfw window");
	}

	if (!s_glew.lock()) {
		Activate();
		s_glew = m_glew = std::make_shared<NG::GLEW>();
	}

	s_window_map[m_window] = this;
	glfwSetScrollCallback(m_window, glfw_scroll_callback);
	glfwSetWindowFocusCallback(m_window, glfw_focus_callback);
}

NG::Window::~Window() {
	s_window_map.erase(m_window);
	glfwDestroyWindow(m_window);
}

void NG::Window::Activate() {
	glfwMakeContextCurrent(m_window);
}

void NG::Window::SetInputMode(int mode, int value) {
	glfwSetInputMode(m_window, mode, value);
}

glm::vec2 NG::Window::GetCursorPos() const {
	double xpos, ypos;
	glfwGetCursorPos(m_window, &xpos, &ypos);
	return {xpos, ypos};
}

void NG::Window::SetCursorPos(int x, int y) {
	glfwSetCursorPos(m_window, x, y);
}

void NG::Window::CenterCursor() {
	SetCursorPos(m_width/2, m_height/2);
}

glm::vec2 NG::Window::GetCenter() const {
	return {m_width/2, m_height/2};
}

void NG::Window::SwapBuffers() {
	glfwSwapBuffers(m_window);
}

bool NG::Window::ShouldClose() const {
	return glfwWindowShouldClose(m_window);
}

bool NG::Window::IsKeyPressed(int key) const {
	return glfwGetKey(m_window, key) == GLFW_PRESS;
}

void NG::Window::ScrollCallback(double xoffset, double yoffset) {
	m_scrolled += glm::vec2(xoffset,yoffset);
}

glm::vec2 NG::Window::GetScrollDistance() const {
	return m_scrolled;
}

void NG::Window::ResetScrollDistance() {
	m_scrolled = glm::vec2(0,0);
}

void NG::Window::FocusCallback(int focused) {
	m_focused = focused;
}

bool NG::Window::IsFocused() const {
	return m_focused;
}
