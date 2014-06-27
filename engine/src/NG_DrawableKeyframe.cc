#include "NG_DrawableKeyframe.hh"

NG::DrawableKeyframe::DrawableKeyframe(std::shared_ptr<NG::VBOKeyframe> model,
																			 std::shared_ptr<NG::Texture> texture,
																			 glm::mat4 model_matrix){
	m_model = model;
	m_texture = texture;
	m_model_matrix = model_matrix;

	m_coefficients.resize(m_model->NumKeyframes());
	m_coefficients[0] = 1;
}

std::shared_ptr<NG::VBO> NG::DrawableKeyframe::GetVBO(){
	return m_model->Interpolate(m_coefficients);
}

std::shared_ptr<NG::Texture> NG::DrawableKeyframe::GetTexture(){
	return m_texture;
}

glm::mat4 NG::DrawableKeyframe::GetModelMatrix(){
	return m_model_matrix;
}
