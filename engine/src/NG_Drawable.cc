#include "NG_Drawable.hh"

void NG::Drawable::Draw(NG::ShaderProgram& shaders, NG::Camera& camera) {
	shaders.Activate();

	auto model = GetModelMatrix();
	auto view = camera.GetView();
	auto perspective = camera.GetPerspective();

	shaders.LoadModelMatrix(model);
	shaders.LoadViewMatrix(view);
	shaders.LoadPerspectiveMatrix(perspective);
	glm::mat4 mvp = perspective * view * model;
	shaders.LoadMVPMatrix(mvp);

	auto vbo = GetVBO();
	auto texture = GetTexture();

	shaders.ActivateTexture(texture);
	vbo->Draw();
}
