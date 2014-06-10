#include <iostream>
using std::cout;
using std::endl;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>

#include "shader.hh"

GLFWwindow* window;

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

  window = glfwCreateWindow(800,600,"First Window",NULL,NULL);
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

  GLuint programID = LoadShaders("shaders/simple.vertex","shaders/simple.fragment");

  GLuint VertexArrayID;
  glGenVertexArrays(1,&VertexArrayID);
  glBindVertexArray(VertexArrayID);

  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
  };
  GLuint vertexbuffer;
  glGenBuffers(1,&vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),g_vertex_buffer_data, GL_STATIC_DRAW);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  do{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES,0,3);
    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while (glfwGetKey(window,GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window)==0);

  glDeleteBuffers(1,&vertexbuffer);
  glDeleteVertexArrays(1, &VertexArrayID);
  glDeleteProgram(programID);

  glfwTerminate();

  cout << "Exiting" << endl;
}
