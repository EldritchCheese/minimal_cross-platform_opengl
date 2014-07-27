#ifndef _NG_TEXT2D_H_
#define _NG_TEXT2D_H_

#include "NG_Shader.hh"
#include "NG_Texture.hh"
#include "NG_GL.hh"

namespace NG {
class Text2D {
public:
	Text2D(const char* vertex_shader_path, const char* fragment_shader_path, const char* texture_path);
	~Text2D();
	void Draw(const char* text, int x, int y, int size);
private:
	ShaderProgram m_shader;
	Texture m_texture;
	GLuint m_vertexbuffer;
	GLuint m_uvbuffer;
};
}

#endif /* _NG_TEXT2D_H_ */
