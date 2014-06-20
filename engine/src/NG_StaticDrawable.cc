#include "NG_StaticDrawable.hh"


NG::StaticDrawable::StaticDrawable(std::shared_ptr<NG::VBO> model,
																	 std::shared_ptr<NG::Texture> texture,
																	 glm::mat4 model_matrix){
	m_model = model;
	m_texture = texture;
	m_model_matrix = model_matrix;
}

std::shared_ptr<NG::VBO> NG::StaticDrawable::GetVBO(){
	return m_model;
}

std::shared_ptr<NG::Texture> NG::StaticDrawable::GetTexture(){
	return m_texture;
}

glm::mat4 NG::StaticDrawable::GetModelMatrix(){
	return m_model_matrix;
}
