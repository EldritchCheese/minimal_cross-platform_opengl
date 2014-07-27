#include "NG_VBO.hh"

#include <cstring>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <vector>
#include <map>
#include <cstdint>

#include "NG_BinaryUtils.hh"

NG::VBO::VBO(const char* file_path){
  std::ifstream infile(file_path, std::ios::in | std::ios::binary);
	init(infile);
}

NG::VBO::VBO(std::ifstream& infile){
	init(infile);
}


NG::VBO::VBO(std::vector<glm::vec3> vertices,
						 std::vector<glm::vec2> uvs,
						 std::vector<glm::vec3> normals,
						 std::vector<unsigned short> indices){
	m_buffers_bound = false;

	m_vertices = vertices;
	m_uvs = uvs;
	m_normals = normals;
	m_indices = indices;
}

void NG::VBO::init(std::ifstream& infile){
	m_buffers_bound = false;

	uint16_t vertex_length;
	binary_read(infile, &vertex_length, 1);
	m_vertices.resize(vertex_length);
	m_uvs.resize(vertex_length);
	m_normals.resize(vertex_length);

	binary_read(infile, m_vertices.data(), vertex_length);
	binary_read(infile, m_uvs.data(), vertex_length);
	binary_read(infile, m_normals.data(), vertex_length);

	uint16_t index_length;
	binary_read(infile, &index_length, 1);
	m_indices.resize(index_length);

	if(index_length % 3){
		throw std::runtime_error("VBO must have 3*n vertices");
	}

	binary_read(infile, m_indices.data(), index_length);

	if(infile.fail()){
		throw std::runtime_error("VBO file ended prematurely");
	}
}

NG::VBO::~VBO(){
  UnbindBuffers();
}

void NG::VBO::BindBuffers(){
  if(m_buffers_bound){
    return;
  }

  m_buffers_bound = true;

  glGenBuffers(1,&m_vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER,m_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(glm::vec3),&m_vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &m_uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, m_uvs.size()*sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);

  glGenBuffers(1,&m_normalbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
  glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);

  glGenBuffers(1, &m_elementbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned short), &m_indices[0], GL_STATIC_DRAW);
}

void NG::VBO::UnbindBuffers(){
  if(!m_buffers_bound){
    return;
  }
  m_buffers_bound = false;
  glDeleteBuffers(1, &m_vertexbuffer);
  glDeleteBuffers(1, &m_uvbuffer);
  glDeleteBuffers(1, &m_normalbuffer);
  glDeleteBuffers(1, &m_elementbuffer);
}

void NG::VBO::Draw() {
  BindBuffers();

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,m_vertexbuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER,m_uvbuffer);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER,m_normalbuffer);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, (void*)0 );

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}
