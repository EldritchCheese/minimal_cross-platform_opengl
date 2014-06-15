#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include <glm/glm.hpp>

#include "NG_Window.hh"
#include "NG_GL.hh"

void computeMatricesFromInputs(NG::Window& window);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif /* _CONTROLS_H_ */
