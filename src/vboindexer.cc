#include "vboindexer.hh"

#include <cmath>
#include <map>
#include <string.h>

template<class T>
bool is_near(T v1, T v2){
  T diff = v1-v2;
  return glm::dot(diff,diff) < 0.001;
}

bool getSimilarVertexIndex(glm::vec3& in_vertex,
			   glm::vec2& in_uv,
			   glm::vec3& in_normal,
			   vector<glm::vec3>& out_vertices,
			   vector<glm::vec2>& out_uvs,
			   vector<glm::vec3>& out_normals,
			   unsigned short& result){
  for(unsigned int i=0; i<out_vertices.size(); i++){
    if(is_near(in_vertex,out_vertices[i]) &&
       is_near(in_uv,out_uvs[i]) &&
       is_near(in_normal,out_normals[i])){
      result = i;
      return true;
    }
  }
  return false;
}

void indexVBO_slow(vector<glm::vec3>& in_vertices,
		   vector<glm::vec2>& in_uvs,
		   vector<glm::vec3>& in_normals,
		   std::vector<unsigned short>& out_indices,
		   vector<glm::vec3>& out_vertices,
		   vector<glm::vec2>& out_uvs,
		   vector<glm::vec3>& out_normals){
  for(unsigned int i=0; i<in_vertices.size(); i++){
    unsigned short index;
    bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i],
				       out_vertices, out_uvs, out_normals,
				       index);
    if(found){
      out_indices.push_back(index);
    } else {
      out_vertices.push_back(in_vertices[i]);
      out_uvs.push_back(in_uvs[i]);
      out_normals.push_back(in_normals[i]);
      out_indices.push_back( (unsigned short)out_vertices.size() - 1);
    }
  }
}

struct PackedVertex{
  glm::vec3 position;
  glm::vec2 uv;
  glm::vec3 normal;
  bool operator<(const PackedVertex that) const{
    return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
  };
};

bool getSimilarVertexIndex_fast(PackedVertex& packed,
				std::map<PackedVertex, unsigned short>& vertex_to_outIndex,
				unsigned short& result){
  std::map<PackedVertex,unsigned short>::iterator it = vertex_to_outIndex.find(packed);
  if(it == vertex_to_outIndex.end() ){
    return false;
  } else {
    result = it->second;
    return true;
  }
}

void indexVBO(vector<glm::vec3>& in_vertices,
	      vector<glm::vec2>& in_uvs,
	      vector<glm::vec3>& in_normals,
	      vector<unsigned short>& out_indices,
	      vector<glm::vec3>& out_vertices,
	      vector<glm::vec2>& out_uvs,
	      vector<glm::vec3>& out_normals){
  std::map<PackedVertex,unsigned short> vertex_to_outIndex;
  for(unsigned int i=0; i<in_vertices.size(); i++){
    PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};
    unsigned short index;
    bool found = getSimilarVertexIndex_fast(packed, vertex_to_outIndex, index);
    if(found){
      out_indices.push_back(index);
    } else {
      out_vertices.push_back(in_vertices[i]);
      out_uvs.push_back(in_uvs[i]);
      out_normals.push_back(in_normals[i]);
      unsigned short newindex = (unsigned short)out_vertices.size() - 1;
      out_indices.push_back(newindex);
      vertex_to_outIndex[packed] = newindex;
    }
  }
}


void indexVBO_TBN(vector<glm::vec3>& in_vertices,
		  vector<glm::vec2>& in_uvs,
		  vector<glm::vec3>& in_normals,
		  vector<glm::vec3>& in_tangents,
		  vector<glm::vec3>& in_bitangents,
		  vector<unsigned short>& out_indices,
		  vector<glm::vec3>& out_vertices,
		  vector<glm::vec2>& out_uvs,
		  vector<glm::vec3>& out_normals,
		  vector<glm::vec3>& out_tangents,
		  vector<glm::vec3>& out_bitangents){
  for(unsigned int i=0; i<in_vertices.size(); i++){
    unsigned short index;
    bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i],
    				       out_vertices, out_uvs, out_normals,
    				       index);
    if(found){
      out_indices.push_back(index);
      out_tangents[index] += in_tangents[i];
      out_bitangents[index] += in_bitangents[i];
    } else {
      out_vertices.push_back(in_vertices[i]);
      out_uvs.push_back(in_uvs[i]);
      out_normals.push_back(in_normals[i]);
      out_tangents.push_back(in_tangents[i]);
      out_bitangents.push_back(in_bitangents[i]);
      out_indices.push_back( (unsigned short)out_vertices.size() - 1 );
    }
  }
}
