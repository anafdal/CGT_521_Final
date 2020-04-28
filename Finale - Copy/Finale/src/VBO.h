#pragma once
//modified from Cherno

class VBO {
private:
	unsigned int bufferID;
public:
	VBO(const void* data, unsigned int size);
	~VBO();//destructive

	void Bind() const;
	void Unbind() const;

};