#include<GL/glew.h>
//modified from Cherno
#include"EBO.h"
#include <iostream>//writing

EBO::EBO(const unsigned int* indices, unsigned int count) {


	glGenBuffers(1, &e_bufferID);//create vertx buffer it's best, first neeed how many buffers
	//then address of bug=ffer which you can use int for

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, e_bufferID);//select buffer; what is the purpose


	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indices, GL_STATIC_DRAW);

}
EBO::~EBO() {
	glDeleteBuffers(1, &e_bufferID);//delete 1 e_bufferID
}
void EBO::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, e_bufferID);
}
void EBO::Unbind()const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}