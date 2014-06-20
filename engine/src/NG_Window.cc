#include "NG_Window.hh"

#include <stdexcept>

namespace {
	static std::weak_ptr<NG::GLFW> s_glfw;
	static std::weak_ptr<NG::GLEW> s_glew;
}

NG::Window::Window(int width, int height, const char* name, GLFWmonitor* monitor, GLFWwindow* share){
  if(!s_glfw.lock()){
    s_glfw = m_glfw = std::make_shared<NG::GLFW>();
  }

  m_width = width;
  m_height = height;
  m_name = name;
  m_window = glfwCreateWindow(m_width, m_height, m_name, monitor, share);
  if(m_window==NULL){
    throw std::runtime_error("Could not make glfw window");
  }

  if(!s_glew.lock()){
    Activate();
    s_glew = m_glew = std::make_shared<NG::GLEW>();
  }
}

NG::Window::~Window(){
  glfwDestroyWindow(m_window);
}

void NG::Window::Activate(){
  glfwMakeContextCurrent(m_window);
}

void NG::Window::SetInputMode(int mode, int value){
  glfwSetInputMode(m_window, mode, value);
}

glm::vec2 NG::Window::GetCursorPos(){
  double xpos, ypos;
  glfwGetCursorPos(m_window, &xpos, &ypos);
  return {xpos, ypos};
}

void NG::Window::SetCursorPos(int x, int y){
  glfwSetCursorPos(m_window, x, y);
}

void NG::Window::CenterCursor(){
  SetCursorPos(m_width/2, m_height/2);
}

glm::vec2 NG::Window::GetCenter(){
  return {m_width/2, m_height/2};
}

void NG::Window::SwapBuffers(){
  glfwSwapBuffers(m_window);
}

bool NG::Window::ShouldClose(){
  return glfwWindowShouldClose(m_window);
}

bool NG::Window::IsKeyPressed(int key){
  return glfwGetKey(m_window, key) == GLFW_PRESS;
}
