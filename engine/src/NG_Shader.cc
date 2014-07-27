#include "NG_Shader.hh"

#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>

#include <GL/glew.h>

NG::ShaderProgram::ShaderProgram(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	Shader vertex_shader(vertex_file_path, GL_VERTEX_SHADER);
	Shader fragment_shader(fragment_file_path, GL_FRAGMENT_SHADER);

	// Link the program
	printf("Linking program\n");
	m_id = glCreateProgram();
	glAttachShader(m_id, vertex_shader.GetID());
	glAttachShader(m_id, fragment_shader.GetID());
	glLinkProgram(m_id);

	// Check the program
	GLint result = GL_FALSE;
	int infoLogLength;
	glGetProgramiv(m_id, GL_LINK_STATUS, &result);
	glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1) {
		std::stringstream ss;
		std::vector<char> error_message(infoLogLength+1);
		glGetProgramInfoLog(m_id, infoLogLength, NULL, &error_message[0]);
		ss << "Error linking shader:\n"
		   << error_message.data();
		throw std::runtime_error(ss.str());
	}
}

NG::ShaderProgram::~ShaderProgram() {
	glDeleteProgram(m_id);
}

void NG::ShaderProgram::Activate() {
	glUseProgram(m_id);
}

GLuint NG::ShaderProgram::GetUniformLocation(const char* name) {
	try {
		return m_uniform_locations.at(name);
	} catch (std::out_of_range e) {
		GLuint location = glGetUniformLocation(m_id,name);
		m_uniform_locations[name] = location;
		return location;
	}
}


void NG::ShaderProgram::LoadUniform(const char* name, glm::mat4& mat) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void NG::ShaderProgram::LoadUniform(const char* name, glm::vec3& vec) {
	LoadUniform(name, vec.x, vec.y, vec.z);
}

void NG::ShaderProgram::LoadUniform(const char* name, double x, double y, double z) {
	glUniform3f(GetUniformLocation(name), x, y, z);
}

void NG::ShaderProgram::LoadUniform(const char* name, GLuint val) {
	glUniform1i(GetUniformLocation(name), val);
}

void NG::ShaderProgram::ActivateTexture(std::shared_ptr<NG::Texture> texture) {
	texture->Activate(GL_TEXTURE0);
	LoadUniform("textureSampler",0);
}

void NG::ShaderProgram::LoadModelMatrix(glm::mat4& mat) {
	LoadUniform("M",mat);
}

void NG::ShaderProgram::LoadViewMatrix(glm::mat4& mat) {
	LoadUniform("V",mat);
}

void NG::ShaderProgram::LoadPerspectiveMatrix(glm::mat4& mat) {
	LoadUniform("P",mat);
}

void NG::ShaderProgram::LoadMVPMatrix(glm::mat4& mat) {
	LoadUniform("MVP",mat);
}


NG::Shader::Shader(const char* file_path, GLuint shader_type) {
	// Read from file
	std::ifstream fileStream(file_path, std::ios::in);
	if (!fileStream.is_open()) {
		std::stringstream ss;
		ss << "Unable to open " << file_path;
		throw std::runtime_error(ss.str());
	}
	std::string code((std::istreambuf_iterator<char>(fileStream)),
	                 std::istreambuf_iterator<char>());
	fileStream.close();

	// Compile
	m_id = glCreateShader(shader_type);
	char const * codePointer = code.c_str();
	glShaderSource(m_id, 1, &codePointer , NULL);
	glCompileShader(m_id);

	// Check
	GLint result = GL_FALSE;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &result);
	int infoLogLength;
	glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1) {
		std::stringstream ss;
		std::vector<char> error_message(infoLogLength+1);
		glGetShaderInfoLog(m_id, infoLogLength, NULL, &error_message[0]);
		ss << "Error compiling shader:\n"
		   << error_message.data();
		throw std::runtime_error(ss.str());
	}

}

NG::Shader::~Shader() {
	glDeleteShader(m_id);
}

GLuint NG::Shader::GetID() {
	return m_id;
}
