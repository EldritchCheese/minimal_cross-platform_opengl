#ifndef _VBOINDEXER_H_
#define _VBOINDEXER_H_

#include <vector>
using std::vector;
#include <glm/glm.hpp>

void indexVBO(vector<glm::vec3>& in_vertices,
	      vector<glm::vec2>& in_uvs,
	      vector<glm::vec3>& in_normals,
	      vector<unsigned short>& out_indices,
	      vector<glm::vec3>& out_vertices,
	      vector<glm::vec2>& out_uvs,
	      vector<glm::vec3>& out_normals);

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
		  vector<glm::vec3>& out_bitangents);

#endif /* _VBOINDEXER_H_ */
