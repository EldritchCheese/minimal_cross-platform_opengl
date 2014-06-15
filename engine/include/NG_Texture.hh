#ifndef _NG_TEXTURE_H_
#define _NG_TEXTURE_H_

#include "NG_GL.hh"

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
