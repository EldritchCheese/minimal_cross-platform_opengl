#ifndef _NGSHADER_H_
#define _NGSHADER_H_

#include <map>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "NG_GL.hh"
#include "NG_Texture.hh"

namespace NG {
class ShaderProgram {
public:
	ShaderProgram(const char* vertex_file_path, const char* fragment_file_path);
	~ShaderProgram();
	void Activate();
	GLuint GetUniformLocation(const char* name);
	void LoadUniform(const char* name, glm::mat4& mat);
	void LoadUniform(const char* name, glm::vec3& vec);
	void LoadUniform(const char* name, double x, double y, double z);
	void LoadUniform(const char* name, GLuint val);

	void ActivateTexture(std::shared_ptr<NG::Texture> texture);
	void LoadModelMatrix(glm::mat4& mat);
	void LoadViewMatrix(glm::mat4& mat);
	void LoadPerspectiveMatrix(glm::mat4& mat);
	void LoadMVPMatrix(glm::mat4& mat);
public:
	GLuint m_id;
	std::map<std::string, GLuint> m_uniform_locations;
	std::shared_ptr<NG::Texture> texture;
};

class Shader {
public:
	Shader(const char* file_path, GLuint shader_type);
	~Shader();
	GLuint GetID();
private:
	GLuint m_id;
};
}

#endif /* _NGSHADER_H_ */
