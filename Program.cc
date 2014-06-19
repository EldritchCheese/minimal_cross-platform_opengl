#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "controls.hh"

#include "NG_GL.hh"
#include "NG_Window.hh"
#include "NG_Shader.hh"
#include "NG_VBO.hh"
#include "NG_Texture.hh"
#include "NG_Text2D.hh"

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
  NG::Texture suzanne_texture("textures/first_model.dds");
  NG::VBO suzanne_vbo("models/first_model.obj");
  NG::Text2D text_overlay("shaders/text.vertex","shaders/text.fragment","textures/Holstein.dds");

  double lastTime = glfwGetTime();
  int nbFrames = 0;
  double ms_per_frame = 0;

  do{

    double currentTime = glfwGetTime();
    nbFrames++;
    if(currentTime - lastTime >= 1.0){
      ms_per_frame = 1000.0/double(nbFrames);
      printf("%f ms/frame\n", ms_per_frame);
      nbFrames = 0;
      lastTime = glfwGetTime();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    standard_shading.Activate();

    computeMatricesFromInputs(window);
    glm::mat4 projection = getProjectionMatrix();
    glm::mat4 view = getViewMatrix();
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),3.14159f/2.0f,glm::vec3(1,0,0));
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),-3.14159f/2.0f,glm::vec3(0,0,1));
    glm::mat4 model = rotateZ * rotateX;
    glm::mat4 mvp = projection * view * model;

    standard_shading.LoadUniform("MVP",mvp);
    standard_shading.LoadUniform("M",model);
    standard_shading.LoadUniform("V",view);


    double time = glfwGetTime();
    double theta = 2*3.14159*time/(3.0);
    glm::vec3 lightPos = glm::vec3(4*cos(theta),4*sin(theta),4);
    standard_shading.LoadUniform("LightPosition_worldspace",lightPos);

    suzanne_texture.Activate(GL_TEXTURE0);
    standard_shading.LoadUniform("textureSampler", 0);

    suzanne_vbo.Draw();

    //Text overlay

    char text[256];
    sprintf(text, "%.2f ms/frame", ms_per_frame );
    text_overlay.Draw(text, 10, 500, 60);

    window.SwapBuffers();
    glfwPollEvents();
  } while (!window.IsKeyPressed(GLFW_KEY_ESCAPE) && !window.ShouldClose());

  cout << "Exiting" << endl;
}
