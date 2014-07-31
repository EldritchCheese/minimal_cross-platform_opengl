#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include <glm/glm.hpp>

#include "NG_Window.hh"
#include "NG_GL.hh"

class FPS_View{
public:
	FPS_View();

	void updateMatrices(NG::Window& window);
	glm::mat4 getViewMatrix(){return m_view;}
	glm::mat4 getProjMatrix(){return m_proj;}
private:
	glm::mat4 m_view;
	glm::mat4 m_proj;
	glm::vec3 m_position;

	double m_lastTime;
	bool m_lastfocus;
	float m_theta;
	float m_phi;
};

#endif /* _CONTROLS_H_ */
