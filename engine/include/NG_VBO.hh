#ifndef _NG_VBO_H_
#define _NG_VBO_H_

#include <vector>
using std::vector;
#include <glm/glm.hpp>

#include "NG_GL.hh"

namespace NG{
  class VBO{
  public:
    VBO(const char* obj_path);
    ~VBO();
    void BindBuffers();
    void UnbindBuffers();
    void Draw();
  private:
    void CustomLoader(const char* obj_path);
    void AssimpLoader(const char* obj_path);

    vector<glm::vec3> m_vertices;
    vector<glm::vec2> m_uvs;
    vector<glm::vec3> m_normals;
    vector<unsigned short> m_indices;

    bool m_buffers_bound;
    GLuint m_vertexbuffer, m_uvbuffer, m_normalbuffer, m_elementbuffer;
  };
}

#endif /* _NG_VBO_H_ */
