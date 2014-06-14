#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

void computeMatricesFromInputs(GLFWwindow* window);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif /* _CONTROLS_H_ */
