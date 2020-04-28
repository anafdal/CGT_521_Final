#pragma once
//modified from Cherno

class EBO {
private:
	unsigned int e_bufferID;
public:
	EBO(const unsigned int* indices,unsigned int count);//need unsigned int for indices
	~EBO();//destructive

	void Bind() const;
	void Unbind() const;

};