#ifndef _NG_DRAWABLEKEYFRAME_H_
#define _NG_DRAWABLEKEYFRAME_H_

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "NG_Drawable.hh"
#include "NG_VBO.hh"
#include "NG_VBOKeyframe.hh"
#include "NG_Texture.hh"

namespace NG {
class DrawableKeyframe : public Drawable {
public:
	DrawableKeyframe(std::shared_ptr<NG::VBOKeyframe> model,
	                 std::shared_ptr<NG::Texture> texture,
	                 glm::mat4 model_matrix);

	virtual std::shared_ptr<NG::VBO> GetVBO();
	virtual std::shared_ptr<NG::Texture> GetTexture();
	virtual glm::mat4 GetModelMatrix();
public:
	std::vector<double> m_coefficients;
private:
	glm::mat4 m_model_matrix;
	std::shared_ptr<NG::VBOKeyframe> m_model;
	std::shared_ptr<NG::Texture> m_texture;
};
}

#endif /* _NG_DRAWABLEKEYFRAME_H_ */
