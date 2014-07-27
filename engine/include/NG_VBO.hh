#ifndef _NG_VBO_H_
#define _NG_VBO_H_

#include <vector>
#include <fstream>

#include <glm/glm.hpp>

#include "NG_GL.hh"

namespace NG{
  class VBO{
  public:
    VBO(const char* file_path);
		VBO(std::ifstream& infile);
		VBO(std::vector<glm::vec3> vertices,
				std::vector<glm::vec2> uvs,
				std::vector<glm::vec3> normals,
				std::vector<unsigned short> indices);
    ~VBO();
    void BindBuffers();
    void UnbindBuffers();
    void Draw();

		const std::vector<glm::vec3>& GetVertices() const {return m_vertices;}
		const std::vector<glm::vec2>& GetUVs() const {return m_uvs;}
		const std::vector<glm::vec3>& GetNormals() const {return m_normals;}
		const std::vector<unsigned short>& GetIndices() const {return m_indices;}
  private:
		void init(std::ifstream& infile);

		std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_uvs;
    std::vector<glm::vec3> m_normals;
    std::vector<unsigned short> m_indices;

    bool m_buffers_bound;
    GLuint m_vertexbuffer, m_uvbuffer, m_normalbuffer, m_elementbuffer;
  };
}

#endif /* _NG_VBO_H_ */
