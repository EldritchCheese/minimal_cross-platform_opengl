#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "controls.hh"

#include "NG_GL.hh"
#include "NG_Window.hh"
#include "NG_Shader.hh"
#include "NG_VBO.hh"
#include "NG_VBOKeyframe.hh"
#include "NG_Texture.hh"
#include "NG_Text2D.hh"
#include "NG_DrawableStatic.hh"
#include "NG_DrawableKeyframe.hh"

int main(){
  cout << "Starting" << endl;

  NG::Window window(800, 600, "Window Class");
  window.Activate();

  window.SetInputMode(GLFW_STICKY_KEYS, GL_TRUE);
  window.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  window.CenterCursor();

  glClearColor(0.0, 0.0, 0.4, 0.0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  NG::ShaderProgram standard_shading("shaders/standard.vertex","shaders/standard.fragment");
  NG::Text2D text_overlay("shaders/text.vertex","shaders/text.fragment","textures/Holstein.dds");


	glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),3.14159f/2.0f,glm::vec3(1,0,0));
	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),-3.14159f/2.0f,glm::vec3(0,0,1));
	glm::mat4 model = rotateZ * rotateX;
	auto suzanne_texture = std::make_shared<NG::Texture>("textures/first_model.dds");
  // auto suzanne_vbo = std::make_shared<NG::VBO>("models/first_model.dat");
	// NG::DrawableStatic suzanne(suzanne_vbo, suzanne_texture, model);
  auto suzanne_vbo = std::make_shared<NG::VBOKeyframe>("models/first_animation.dat");
	NG::DrawableKeyframe suzanne(suzanne_vbo, suzanne_texture, model);


  double lastTime = glfwGetTime();
  int nbFrames = 0;
  double ms_per_frame = 0;

  do{

    double time = glfwGetTime();
    nbFrames++;
    if(time - lastTime >= 1.0){
      ms_per_frame = 1000.0/double(nbFrames);
      printf("%f ms/frame\n", ms_per_frame);
      nbFrames = 0;
      lastTime = glfwGetTime();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    standard_shading.Activate();
    double theta = 2*3.14159*time/(3.0);
    glm::vec3 lightPos = glm::vec3(10*cos(theta),10*sin(theta),10);
    standard_shading.LoadUniform("LightPosition_worldspace",lightPos);

    computeMatricesFromInputs(window);
    glm::mat4 projection = getProjectionMatrix();
    glm::mat4 view = getViewMatrix();
		NG::Camera camera(view,projection);

		suzanne.m_coefficients[0] = 1+cos(2*theta);
		suzanne.m_coefficients[1] = 1-cos(2*theta);
		suzanne.Draw(standard_shading, camera);

    //Text overlay

    char text[256];
    sprintf(text, "%.2f ms/frame", ms_per_frame );
    text_overlay.Draw(text, 10, 500, 60);

    window.SwapBuffers();
    glfwPollEvents();
  } while (!window.IsKeyPressed(GLFW_KEY_ESCAPE) && !window.ShouldClose());

  cout << "Exiting" << endl;
}
