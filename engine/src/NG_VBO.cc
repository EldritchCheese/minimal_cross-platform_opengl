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

NG::VBO::VBO(const char* obj_path){
  m_buffers_bound = false;

  FILE* file = fopen(obj_path,"r");
  if(file==NULL){
    std::stringstream ss;
    ss << "Could not open obj file: " << obj_path;
    throw std::runtime_error(ss.str());
  }

  std::vector<std::tuple<uint,uint,uint> > temp_indices;
  std::vector<glm::vec3> temp_vertices;
  std::vector<glm::vec2> temp_uvs;
  std::vector<glm::vec3> temp_normals;

  while(true){
    char lineHeader[128];
    int res = fscanf(file, "%s", lineHeader);
    if(res==EOF){
      break;
    }

    if(strcmp(lineHeader, "v") == 0){
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      temp_vertices.push_back(vertex);
    } else if(strcmp(lineHeader, "vt") == 0){
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      uv.y = -uv.y;
      temp_uvs.push_back(uv);
    } else if(strcmp(lineHeader, "vn") == 0){
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      temp_normals.push_back(normal);
    } else if(strcmp(lineHeader,"f") == 0){
      std::string vertex1, vertex2, vertex3;
      uint vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
			   &vertexIndex[0], &uvIndex[0], &normalIndex[0],
			   &vertexIndex[1], &uvIndex[1], &normalIndex[1],
			   &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
      if(matches!=9){
	throw std::runtime_error("Parsing failure");
      }

      temp_indices.push_back( std::make_tuple(vertexIndex[0], uvIndex[0], normalIndex[0]) );
      temp_indices.push_back( std::make_tuple(vertexIndex[1], uvIndex[1], normalIndex[1]) );
      temp_indices.push_back( std::make_tuple(vertexIndex[2], uvIndex[2], normalIndex[2]) );

    } else {
      char dumpBuffer[1000];
      fgets(dumpBuffer, 1000, file);
    }
  }

  uint vIndex, uvIndex, nIndex;
  std::map<std::tuple<uint,uint,uint>, uint> objIndex_to_index;
  for(const auto& objIndex : temp_indices){
    try{
      m_indices.push_back( objIndex_to_index.at(objIndex) );
    } catch(std::out_of_range e) {
      uint index = m_vertices.size();
      objIndex_to_index[objIndex] = index;

      std::tie(vIndex, uvIndex, nIndex) = objIndex;
      m_indices.push_back(index);
      // OBJ indices are 1-indexed
      m_vertices.push_back(temp_vertices[vIndex-1]);
      m_uvs.push_back(temp_uvs[uvIndex-1]);
      m_normals.push_back(temp_normals[nIndex-1]);
    }
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
