#ifndef _NG_CAMERA_H_
#define _NG_CAMERA_H_

#include <glm/glm.hpp>

namespace NG {
class Camera {
public:
	Camera();
	Camera(const glm::mat4& view, const glm::mat4& perspective);

	glm::mat4 GetView();
	glm::mat4 GetPerspective();
private:
	glm::mat4 m_view;
	glm::mat4 m_perspective;
};
}

#endif /* _NG_CAMERA_H_ */
