#ifndef _NG_STATICDRAWABLE_H_
#define _NG_STATICDRAWABLE_H_

#include <memory>

#include "NG_Drawable.hh"
#include "NG_Texture.hh"
#include "NG_VBO.hh"

namespace NG {
	class StaticDrawable : public Drawable{
	public:
		StaticDrawable(std::shared_ptr<NG::VBO> model,
									 std::shared_ptr<NG::Texture> texture,
									 glm::mat4 model_matrix);

		virtual std::shared_ptr<NG::VBO> GetVBO();
		virtual std::shared_ptr<NG::Texture> GetTexture();
		virtual glm::mat4 GetModelMatrix();

	private:
		glm::mat4 m_model_matrix;
		std::shared_ptr<NG::VBO> m_model;
		std::shared_ptr<NG::Texture> m_texture;
	};
}

#endif /* _NG_STATICDRAWABLE_H_ */
