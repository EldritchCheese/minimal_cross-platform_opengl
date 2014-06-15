#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "NG_Window.hh"

void computeMatricesFromInputs(NG::Window& window);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif /* _CONTROLS_H_ */
