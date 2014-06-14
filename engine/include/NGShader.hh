#ifndef _NGSHADER_H_
#define _NGSHADER_H_

#include <GL/glew.h>

namespace NG{
  class ShaderProgram{
  public:
    ShaderProgram(const char* vertex_file_path, const char* fragment_file_path);
    ~ShaderProgram();
    void Activate();
  public:
    GLuint m_id;
  };

  class Shader{
  public:
    Shader(const char* file_path, GLuint shader_type);
    ~Shader();
    GLuint GetID();
  private:
    GLuint m_id;
  };
}

#endif /* _NGSHADER_H_ */
