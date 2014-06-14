#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GL/glew.h>

GLuint loadBMP_custom(const char* imagepath);
GLuint loadDDS(const char* imagepath);

#endif /* _TEXTURE_H_ */
