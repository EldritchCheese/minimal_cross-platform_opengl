#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hh"
#include "controls.hh"
#include "text2D.hh"

#include "NG_Window.hh"
#include "NG_Shader.hh"
#include "NG_VBO.hh"

int main(){
  cout << "Starting" << endl;

  if(!glfwInit()){
    cout << "Failed to initialize GLFW" << endl;
    return -1;
  }

  // glfwWindowHint(GLFW_SAMPLES,4);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  NG::Window window(800, 600, "Window Class");
  window.Activate();

  glewExperimental = true;
  if(glewInit() != GLEW_OK){
    cout << "Coudn't initialize GLEW" << endl;
    return -1;
  }
  window.SetInputMode(GLFW_STICKY_KEYS, GL_TRUE);
  window.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  window.CenterCursor();

  glClearColor(0.0, 0.0, 0.4, 0.0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLuint VertexArrayID;
  glGenVertexArrays(1,&VertexArrayID);
  glBindVertexArray(VertexArrayID);

  NG::ShaderProgram standard_shading("resources/standard_330.vertex","resources/standard_330.fragment");

  GLuint matrixID = glGetUniformLocation(standard_shading.m_id,"MVP");
  GLuint viewMatrixID = glGetUniformLocation(standard_shading.m_id,"V");
  GLuint modelMatrixID = glGetUniformLocation(standard_shading.m_id,"M");

  GLuint texture = loadDDS("resources/suzanne.DDS");
  GLuint textureID = glGetUniformLocation(standard_shading.m_id,"textureSampler");

  NG::VBO suzanne("resources/suzanne.obj");

  standard_shading.Activate();
  GLuint lightID = glGetUniformLocation(standard_shading.m_id, "LightPosition_worldspace");

  initText2D("resources/Holstein.DDS");

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

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &view[0][0]);

    double time = glfwGetTime();
    double theta = 2*3.14159*time/(3.0);
    glm::vec3 lightPos = glm::vec3(4*cos(theta),4*sin(theta),4);
    glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);

    // Draw first object

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureID, 0);

    suzanne.Draw();

    // Draw second object

    model = glm::translate(model, glm::vec3(2.5,0,0));
    mvp = projection * view * model;

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &model[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureID, 0);

    suzanne.Draw();

    // Clean up

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    char text[256];
    sprintf(text, "%.2f ms/frame", ms_per_frame );
    printText2D(text, 10, 500, 60);

    window.SwapBuffers();
    glfwPollEvents();
  } while (!window.IsKeyPressed(GLFW_KEY_ESCAPE) && !window.ShouldClose());

  // glDeleteBuffers(1,&vertexbuffer);
  // glDeleteBuffers(1,&uvbuffer);
  // glDeleteBuffers(1, &normalbuffer);
  // glDeleteBuffers(1, &elementbuffer);
  glDeleteTextures(1, &textureID);
  glDeleteVertexArrays(1, &VertexArrayID);

  glfwTerminate();

  cout << "Exiting" << endl;
}
