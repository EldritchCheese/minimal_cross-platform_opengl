#ifndef _NG_GL_H_
#define _NG_GL_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace NG {
  class GLFW {
  public:
    GLFW();
    ~GLFW();
  };
  class GLEW {
  public:
    GLEW();
  };
}

#endif /* _NG_GL_H_ */
