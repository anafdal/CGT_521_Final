#include<GL/glew.h>
//modified from Cherno
#include"VBO.h"
#include <iostream>//writing

VBO::VBO(const void* data, unsigned int size) {


	glGenBuffers(1, &bufferID);//create vertx buffer it's best, first neeed how many buffers
	//then address of bug=ffer which you can use int for

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);//select buffer; what is the purpose
	//what type of buffer and who to generate it
	//now put data
	//can just give size and later data or give data now

	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	
}
VBO::~VBO() {
	glDeleteBuffers(1, &bufferID);//delete 1 bufferID
}
void VBO::Bind()const {
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
}
void VBO::Unbind()const{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}