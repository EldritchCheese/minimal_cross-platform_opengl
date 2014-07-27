#include "texture.hh"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint loadBMP_custom(const char* imagepath) {
	printf("Reading image %s\n", imagepath);

	FILE* file = fopen(imagepath,"rb");
	if (!file) {
		printf("%s could not be opened",imagepath);
		getchar();
		return 0;
	}

	unsigned char header[54];
	if (fread(header,1,54,file)!=54) {
		printf("Incorrect BMP format\n");
		return 0;
	}

	if (header[0]!='B' || header[1]!='M') {
		printf("Wrong BMP header\n");
		return 0;
	}

	if ((*(int*) &(header[0x1E]) != 0) ||
	        (*(int*) &(header[0x1C]) != 24)) {
		printf("Expected 24bpp BMP file\n");
		return 0;
	}

	unsigned int dataPos = *(int*)&(header[0x0A]);
	unsigned int imageSize = *(int*)&(header[0x22]);
	unsigned int width = *(int*)&(header[0x12]);
	unsigned int height = *(int*)&(header[0x16]);

	if (imageSize==0) {
		imageSize = width*height*3;
	}
	if (dataPos==0) {
		dataPos = 54;
	}

	unsigned char* data = new unsigned char[imageSize];
	fread(data,1,imageSize,file);
	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	delete[] data;

	//Trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
GLuint loadDDS(const char * imagepath) {
	FILE* fp = fopen(imagepath, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
		getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
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
	unsigned char* buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC) {
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
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
		                       0, size, buffer + offset);

		offset += size;
		width  /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) { width = 1; }
		if (height < 1) { height = 1; }

	}

	free(buffer);

	return textureID;
}
