#include "NG_DrawableStatic.hh"


NG::DrawableStatic::DrawableStatic(std::shared_ptr<NG::VBO> model,
																	 std::shared_ptr<NG::Texture> texture,
																	 glm::mat4 model_matrix){
	m_model = model;
	m_texture = texture;
	m_model_matrix = model_matrix;
}

std::shared_ptr<NG::VBO> NG::DrawableStatic::GetVBO(){
	return m_model;
}

std::shared_ptr<NG::Texture> NG::DrawableStatic::GetTexture(){
	return m_texture;
}

glm::mat4 NG::DrawableStatic::GetModelMatrix(){
	return m_model_matrix;
}
