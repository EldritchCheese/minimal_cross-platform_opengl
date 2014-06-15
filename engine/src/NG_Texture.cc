#include "NG_Texture.hh"

#include <stdexcept>
#include <sstream>
#include <vector>
#include <cstring>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII


NG::Texture::Texture(const char* dds_path){
  FILE* fp = fopen(dds_path, "rb");
  if (fp == NULL){
    std::stringstream ss;
    ss << "File not found: " << dds_path;
    throw std::runtime_error(ss.str());
  }

  /* verify the type of file */
  char filecode[4];
  fread(filecode, 1, 4, fp);
  if (strncmp(filecode, "DDS ", 4) != 0) {
    fclose(fp);
    throw std::runtime_error("Wrong DDS header");
  }

  /* get the surface desc */
  unsigned char header[124];
  fread(&header, 124, 1, fp);

  unsigned int height      = *(unsigned int*)&(header[8 ]);
  unsigned int width       = *(unsigned int*)&(header[12]);
  unsigned int linearSize  = *(unsigned int*)&(header[16]);
  unsigned int mipMapCount = *(unsigned int*)&(header[24]);
  unsigned int fourCC      = *(unsigned int*)&(header[80]);


  /* how big is it going to be including all mipmaps? */
  unsigned int bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
  std::vector<unsigned char> buffer(bufsize);
  fread(&buffer[0], 1, bufsize, fp);
  /* close the file pointer */
  fclose(fp);

  unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
  unsigned int format;
  switch(fourCC) {
  case FOURCC_DXT1:
    format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    break;
  case FOURCC_DXT3:
    format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    break;
  case FOURCC_DXT5:
    format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    break;
  default:
    throw std::runtime_error("Unknown DDS compression");
  }

  // Create one OpenGL texture
  glGenTextures(1, &m_id);

  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, m_id);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);

  unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
  unsigned int offset = 0;

  /* load the mipmaps */
  for (unsigned int level = 0; level < mipMapCount && (width || height); level++) {
    unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
    glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			   0, size, &buffer[0] + offset);

    offset += size;
    width  /= 2;
    height /= 2;

    // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
    if(width < 1) width = 1;
    if(height < 1) height = 1;
  }
}

NG::Texture::~Texture(){
  glDeleteTextures(1, &m_id);
}

void NG::Texture::Activate(GLenum texture_num){
  glActiveTexture(texture_num);
  glBindTexture(GL_TEXTURE_2D, m_id);
}
