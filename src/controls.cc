#include "controls.hh"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 viewMatrix;
glm::mat4 projMatrix;

glm::mat4 getViewMatrix(){
  return viewMatrix;
}
glm::mat4 getProjectionMatrix(){
  return projMatrix;
}

glm::vec3 position = glm::vec3(-5, 0, 0);
float theta = 3.14f/2.0f;
float phi = 0.0f;
float initialFoV = 45.0f * (3.14159f/180.0f);

float speed = 3.0f;
float mouseSpeed = 0.005f;

void computeMatricesFromInputs(GLFWwindow* window){
  static double lastTime = glfwGetTime();

  double currentTime = glfwGetTime();
  float deltaTime = float(currentTime - lastTime);

  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  glfwSetCursorPos(window, 800/2, 600/2);

  phi   -= mouseSpeed * float(xpos - 800/2);
  theta += mouseSpeed * float(ypos - 600/2);
  if(theta<0){
    theta = 0;
  } else if (theta>3.14f){
    theta = 3.14f;
  }

  glm::vec3 direction(sin(theta) * cos(phi),
		      sin(theta) * sin(phi),
		      cos(theta) );
  glm::vec3 right(sin(phi), -cos(phi), 0);
  glm::vec3 up = glm::cross(right, direction);

  glm::vec3 z(0,0,1);

  if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS){
    position += direction * deltaTime * speed;
  }
  if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS){
    position -= direction * deltaTime * speed;
  }
  if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS){
    position -= right * deltaTime * speed;
  }
  if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS){
    position += right * deltaTime * speed;
  }
  if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS){
    position += z * deltaTime * speed;
  }
  if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS){
    position -= z * deltaTime * speed;
  }

  float FoV = initialFoV;

  projMatrix = glm::perspective(FoV, 4.0f/3.0f, 0.1f, 100.0f);
  viewMatrix = glm::lookAt(position, position+direction, up);

  lastTime = currentTime;
}
