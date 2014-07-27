#include "NG_Texture.hh"

#include <stdexcept>
#include <sstream>
#include <vector>
#include <cstring>

#include "SOIL.h"

NG::Texture::Texture(const char* path){
	if(!glfwGetCurrentContext()){
		throw std::runtime_error("No GL context when loading texture");
	}

  m_id = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
															 //			       SOIL_FLAG_DDS_LOAD_DIRECT);
															 SOIL_FLAG_INVERT_Y);

	if(m_id==0){
		std::stringstream ss;
		ss << "Could not load texture \"" << path << "\"";
		throw std::runtime_error(ss.str());
	}
}

NG::Texture::~Texture(){
  glDeleteTextures(1, &m_id);
}

void NG::Texture::Activate(GLenum texture_num){
  glActiveTexture(texture_num);
  glBindTexture(GL_TEXTURE_2D, m_id);
}
