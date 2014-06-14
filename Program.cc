#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hh"
#include "texture.hh"
#include "controls.hh"
#include "objloader.hh"
#include "vboindexer.hh"
#include "text2D.hh"

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

  GLFWwindow* window = glfwCreateWindow(800,600,"First Window",NULL,NULL);
  if(!window){
    cout << "Couldn't open GLFW window" << endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);


  glewExperimental = true;
  if(glewInit() != GLEW_OK){
    cout << "Coudn't initialize GLEW" << endl;
    return -1;
  }
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwSetCursorPos(window, 800/2, 600/2);

  glClearColor(0.0, 0.0, 0.4, 0.0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLuint VertexArrayID;
  glGenVertexArrays(1,&VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GLuint programID = LoadShaders("resources/standard_330.vertex","resources/standard_330.fragment");

  GLuint matrixID = glGetUniformLocation(programID,"MVP");
  GLuint viewMatrixID = glGetUniformLocation(programID,"V");
  GLuint modelMatrixID = glGetUniformLocation(programID,"M");

  GLuint texture = loadDDS("resources/suzanne.DDS");
  GLuint textureID = glGetUniformLocation(programID,"textureSampler");

  vector<glm::vec3> vertices;
  vector<glm::vec2> uvs;
  vector<glm::vec3> normals;
  bool res = loadOBJ("resources/suzanne.obj",vertices,uvs,normals);

  vector<unsigned short> indices;
  vector<glm::vec3> indexed_vertices;
  vector<glm::vec2> indexed_uvs;
  vector<glm::vec3> indexed_normals;
  indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

  GLuint vertexbuffer;
  glGenBuffers(1,&vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size()*sizeof(glm::vec3),&indexed_vertices[0], GL_STATIC_DRAW);

  GLuint uvbuffer;
  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size()*sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

  GLuint normalbuffer;
  glGenBuffers(1,&normalbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

  GLuint elementbuffer;
  glGenBuffers(1, &elementbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

  glUseProgram(programID);
  GLuint lightID = glGetUniformLocation(programID, "LightPosition_worldspace");

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

    glUseProgram(programID);

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

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER,normalbuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0 );

    // Draw second object

    model = glm::translate(model, glm::vec3(2.5,0,0));
    mvp = projection * view * model;

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &model[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureID, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER,normalbuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0 );

    // Clean up

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    char text[256];
    sprintf(text, "%.2f ms/frame", ms_per_frame );
    printText2D(text, 10, 500, 60);

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while (glfwGetKey(window,GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window)==0);

  glDeleteBuffers(1,&vertexbuffer);
  glDeleteBuffers(1,&uvbuffer);
  glDeleteBuffers(1, &normalbuffer);
  glDeleteBuffers(1, &elementbuffer);
  glDeleteProgram(programID);
  glDeleteTextures(1, &textureID);
  glDeleteVertexArrays(1, &VertexArrayID);

  glfwTerminate();

  cout << "Exiting" << endl;
}
