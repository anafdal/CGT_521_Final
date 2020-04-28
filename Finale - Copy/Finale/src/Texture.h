#pragma once
#include <iostream>//writing
//modified from Cherno

class Texture {

private:
	unsigned int textureId;
	std::string filename;
	unsigned char* localBuffer;

	int width, height, bpp;
public:
	Texture(const std::string& filename);
	~Texture();

	void  Bind() const;
	void  Unbind() const;

	//get width and height just in case you need them
	inline int GetWidth() const { return width; };
	inline int GetHeight() const { return height; };


};