#include "Texture.h"
#include <glfw3.h>
#include <iostream>
#include "vendor/stb_image.h"
//modified from Cherno

Texture::Texture(const std::string& filename) {

	//data
	stbi_set_flip_vertically_on_load(1);//flips texture vertically up side down because of png
	localBuffer = stbi_load(filename.c_str(), &width, &height,&bpp,4);//pass pointers so function can write them

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	//texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);//horizontal wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);//vertical wrap


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (localBuffer)
		stbi_image_free(localBuffer);
}
Texture::~Texture() {

	glDeleteTextures(1,&textureId);
}

void  Texture::Bind() const {

	glBindTexture(GL_TEXTURE_2D, textureId);

}
void  Texture::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}