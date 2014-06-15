#include "NG_Shader.hh"

#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>

#include <GL/glew.h>

NG::ShaderProgram::ShaderProgram(const char* vertex_file_path, const char* fragment_file_path){

  // Create the shaders
  Shader vertex_shader(vertex_file_path, GL_VERTEX_SHADER);
  Shader fragment_shader(fragment_file_path, GL_FRAGMENT_SHADER);

  // Link the program
  printf("Linking program\n");
  m_id = glCreateProgram();
  glAttachShader(m_id, vertex_shader.GetID());
  glAttachShader(m_id, fragment_shader.GetID());
  glLinkProgram(m_id);

  // Check the program
  GLint result = GL_FALSE;
  int infoLogLength;
  glGetProgramiv(m_id, GL_LINK_STATUS, &result);
  glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
  if ( infoLogLength > 0 ){
    std::vector<char> error_message(infoLogLength+1);
    glGetProgramInfoLog(m_id, infoLogLength, NULL, &error_message[0]);
    throw std::runtime_error(error_message.data());
  }
}

NG::ShaderProgram::~ShaderProgram(){
  glDeleteProgram(m_id);
}

void NG::ShaderProgram::Activate(){
  glUseProgram(m_id);
}

NG::Shader::Shader(const char* file_path, GLuint shader_type){
  // Read from file
  std::ifstream fileStream(file_path, std::ios::in);
  if(!fileStream.is_open()){
    std::stringstream ss;
    ss << "Unable to open " << file_path;
    throw std::runtime_error(ss.str());
  }
  std::string code((std::istreambuf_iterator<char>(fileStream)),
		   std::istreambuf_iterator<char>());
  fileStream.close();

  // Compile
  m_id = glCreateShader(shader_type);
  char const * codePointer = code.c_str();
  glShaderSource(m_id, 1, &codePointer , NULL);
  glCompileShader(m_id);

  // Check
  GLint result = GL_FALSE;
  glGetShaderiv(m_id, GL_COMPILE_STATUS, &result);
  int infoLogLength;
  glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
  if ( infoLogLength > 0 ){
    std::vector<char> error_message(infoLogLength+1);
    glGetShaderInfoLog(m_id, infoLogLength, NULL, &error_message[0]);
    throw std::runtime_error(error_message.data());
  }

}

NG::Shader::~Shader(){
  glDeleteShader(m_id);
}

GLuint NG::Shader::GetID(){
  return m_id;
}
