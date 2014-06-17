#include "NG_VBO.hh"

#include <cstring>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <vector>
#include <map>
#include <iostream>
using namespace std;
typedef unsigned int uint;

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

NG::VBO::VBO(const char* obj_path){
  m_buffers_bound = false;

  AssimpLoader(obj_path);
}

void NG::VBO::AssimpLoader(const char* obj_path){
  Assimp::Importer importer;
  // scene is owned by importer
  const aiScene* scene = importer.ReadFile(obj_path, 0);
  if(!scene){
    std::stringstream ss;
    ss << "Could not read " << obj_path;
    throw std::runtime_error(ss.str());
  }
  const aiMesh* mesh = scene->mMeshes[0];

  m_vertices.reserve(mesh->mNumVertices);
  for(unsigned int i=0; i<mesh->mNumVertices; i++){
    auto pos = mesh->mVertices[i];
    m_vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
  }

  m_uvs.reserve(mesh->mNumVertices);
  for(unsigned int i=0; i<mesh->mNumVertices; i++){
    auto uvw = mesh->mTextureCoords[0][i];
    m_uvs.push_back(glm::vec2(uvw.x, -uvw.y));
  }

  m_normals.reserve(mesh->mNumVertices);
  for(unsigned int i=0; i<mesh->mNumVertices; i++){
    auto norm = mesh->mNormals[i];
    m_normals.push_back(glm::vec3(norm.x, norm.y, norm.z));
  }

  m_indices.reserve(3*mesh->mNumFaces);
  for(unsigned int i=0; i<mesh->mNumFaces; i++){
    m_indices.push_back(mesh->mFaces[i].mIndices[0]);
    m_indices.push_back(mesh->mFaces[i].mIndices[1]);
    m_indices.push_back(mesh->mFaces[i].mIndices[2]);
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

void NG::VBO::Draw(){
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
