#ifndef _NG_TEXTURE_H_
#define _NG_TEXTURE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace NG {
  class Texture{
  public:
    Texture(const char* dds_path);
    ~Texture();
    void Activate(GLenum texture_num);
  private:
    GLuint m_id;
  };
}

#endif /* _NG_TEXTURE_H_ */
