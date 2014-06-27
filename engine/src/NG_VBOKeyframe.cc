#include "NG_VBOKeyframe.hh"

#include <cstdint>
#include <sstream>
#include <stdexcept>

#include "NG_BinaryUtils.hh"

NG::VBOKeyframe::VBOKeyframe(const char* file_path){
	std::ifstream infile(file_path, std::ios::in | std::ios::binary);
	init(infile);
}

NG::VBOKeyframe::VBOKeyframe(std::ifstream& infile){
	init(infile);
}

void NG::VBOKeyframe::init(std::ifstream& infile){
	uint16_t numframes;
	binary_read(infile, &numframes, 1);

	if(!numframes){
		throw std::runtime_error("VBOKeyframe must have at least one keyframe");
	}

	for(unsigned int i=0; i<numframes; i++){
		m_keyframes.push_back(std::make_shared<NG::VBO>(infile));
	}

	unsigned int num_indices = m_keyframes[0]->GetIndices().size();
	for(auto vbo : m_keyframes){
		if(m_keyframes[0]->GetIndices().size() != num_indices){
			throw std::runtime_error("Keyframes in VBO must all have same indices");
		}
	}
}

std::shared_ptr<NG::VBO> NG::VBOKeyframe::Interpolate(std::vector<double> coefficients){
	if(coefficients.size() != m_keyframes.size()){
		std::stringstream ss;
		ss << "Expected " << m_keyframes.size() << " coefficients but received " << coefficients.size();
		throw std::runtime_error(ss.str());
	}

	double sum_coefficients = 0;
	for(const auto& val : coefficients){
		sum_coefficients += val;
	}

	if(sum_coefficients==0){
		throw std::runtime_error("Must have non-zero coefficients for Keyframe interpolation");
	}

	unsigned int total_vertices = m_keyframes[0]->GetVertices().size();

	std::vector<glm::vec3> vertices(total_vertices);
	std::vector<glm::vec2> uvs(total_vertices);
	std::vector<glm::vec3> normals(total_vertices);

	for(unsigned int keyframe_num=0; keyframe_num<m_keyframes.size(); keyframe_num++){
		float coefficient = coefficients[keyframe_num]/sum_coefficients;
		auto keyf_vertices = m_keyframes[keyframe_num]->GetVertices();
		auto keyf_uvs = m_keyframes[keyframe_num]->GetUVs();
		auto keyf_normals = m_keyframes[keyframe_num]->GetNormals();
		for(unsigned int vertex_num=0; vertex_num<total_vertices; vertex_num++){
			vertices[vertex_num] += coefficient * keyf_vertices.at(vertex_num);
			uvs[vertex_num]      += coefficient * keyf_uvs.at(vertex_num);
			normals[vertex_num]  += coefficient * keyf_normals.at(vertex_num);
		}
	}

	return std::make_shared<NG::VBO>(vertices, uvs, normals, m_keyframes[0]->GetIndices());
}
