#include "NG_Text2D.hh"

#include <cstring>
#include <vector>
using std::vector;

#include <glm/glm.hpp>

NG::Text2D::Text2D(const char* vertex_shader_path, const char* fragment_shader_path, const char* texture_path) :
  m_shader(vertex_shader_path, fragment_shader_path), m_texture(texture_path) {

  glGenBuffers(1, &m_vertexbuffer);
  glGenBuffers(1, &m_uvbuffer);
}

NG::Text2D::~Text2D(){
  glDeleteBuffers(1, &m_vertexbuffer);
  glDeleteBuffers(1, &m_uvbuffer);
}

void NG::Text2D::Draw(const char* text, int x, int y, int size){
  unsigned int length = strlen(text);

  // Generate letter locations
  vector<glm::vec2> vertices;
  vector<glm::vec2> UVs;
  for(unsigned int i=0; i<length; i++){
    glm::vec2 vertex_up_left = glm::vec2( x+i*size, y+size);
    glm::vec2 vertex_up_right = glm::vec2( x+(i+1)*size, y+size);
    glm::vec2 vertex_down_left = glm::vec2( x+i*size, y);
    glm::vec2 vertex_down_right = glm::vec2( x+(i+1)*size, y);

    char character = text[i];
    float uv_x = (character%16) / 16.0f;
    float uv_y = (character/16) / 16.0f;

    glm::vec2 uv_up_left = glm::vec2(uv_x, 1-uv_y);
    glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f/16.0f, 1-uv_y);
    glm::vec2 uv_down_left = glm::vec2(uv_x, 1-(uv_y + 1.0f/16.0f));
    glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f/16.0f, 1-(uv_y + 1.0f/16.0f));

    vertices.push_back(vertex_up_left);
    UVs.push_back(uv_up_left);
    vertices.push_back(vertex_down_left);
    UVs.push_back(uv_down_left);
    vertices.push_back(vertex_up_right);
    UVs.push_back(uv_up_right);

    vertices.push_back(vertex_down_right);
    UVs.push_back(uv_down_right);
    vertices.push_back(vertex_up_right);
    UVs.push_back(uv_up_right);
    vertices.push_back(vertex_down_left);
    UVs.push_back(uv_down_left);
  }


  glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, UVs.size()*sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

  m_shader.Activate();

  m_texture.Activate(GL_TEXTURE0);
  m_shader.LoadUniform("textureSampler", 0);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDrawArrays(GL_TRIANGLES, 0, vertices.size());

  glDisable(GL_BLEND);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
