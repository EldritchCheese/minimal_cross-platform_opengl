#ifndef _NG_DRAWABLE_H_
#define _NG_DRAWABLE_H_

#include <memory>

#include <glm/glm.hpp>

#include "NG_VBO.hh"
#include "NG_Shader.hh"
#include "NG_Texture.hh"
#include "NG_Camera.hh"

namespace NG {
	class Drawable{
	public:
		virtual std::shared_ptr<NG::VBO> GetVBO() = 0;
		virtual std::shared_ptr<NG::Texture> GetTexture() = 0;
		virtual glm::mat4 GetModelMatrix() = 0;

		virtual void Draw(NG::ShaderProgram& shaders, NG::Camera& camera);
	};
}

#endif /* _NG_DRAWABLE_H_ */
