#include<GL/glew.h>
#include "Shader.h"


#include <iostream>
#include <fstream>
#include <string>

#include <sstream>
#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//modified from Cherno


const std::string Shader::ParseShader1(const std::string& filepath) {//modified from The Cherno

	std::ifstream stream(filepath);//go through file line by line

	std::string line;
	std::stringstream ss;

	while (getline(stream, line)) {

		ss << line << '\n';
	}
	return ss.str();
}

unsigned int Shader::CompileShader(const std::string source, unsigned int type) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();//pointer to beginning of data;&source[0];pointer to data in string
	glShaderSource(id, 1, &src, nullptr);//take id, count,point to source,length
	glCompileShader(id);//code to compile shader

	//Error Handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);//i-integer,v-vector
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));//stack allocated length

		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ?
			"vertex" : "fragment")
			<< std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;

	}

	return id;


}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	//use shaders here
	///need shader souce code and text
	unsigned int program = glCreateProgram();//returns unsigned int



	unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);//vs
	unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);//fs

	//attach shaders tp program
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);//delete intermediates

	return program;
}


Shader::Shader(const std::string& filename1, const std::string& filename2)
{
	const std::string vshader = ParseShader1(filename1);//vertex shader
	const std::string fshader = ParseShader1(filename2);//fragment shader
	shaderId = CreateShader(vshader, fshader);
}

Shader::~Shader() {
	glDeleteProgram(shaderId);
}

void Shader::Bind()const {

	glUseProgram(shaderId);
}
void Shader::Unbind()const {
	glUseProgram(0);
}

//set uniforms
void Shader::SetUniform4f(const std::string&& name, float f0, float f1, float f2, float f3) {

	glUniform4f(GetUniformLocation(name.c_str()), f0, f1, f2, f3);
}


 int Shader::GetUniformLocation(const std::string&& name) {

	if(locationCache.find(name)!=locationCache.end())//does it already contain it
		return locationCache[name];

	
	int location=glGetUniformLocation(shaderId, name.c_str());
	if (location == -1) {
		
		std::cout << "Warning: uniform " << name << " doesn't exist." << std::endl;
	}
	else {
		locationCache[name] = location;//if not conatined already then you can use it
	}

	return location;

}

void Shader::SetUniform1f(const std::string&& name, float f) {

	glUniform1f(GetUniformLocation(name.c_str()), f);
}

void Shader::SetUniform1i(const std::string&& name, int i) {

	glUniform1i(GetUniformLocation(name.c_str()), i);
}
void Shader::SetUniformMatrix4fv(const std::string&& name, const glm::mat4& matrix) {

	glUniformMatrix4fv(GetUniformLocation(name.c_str()), 1, false, glm::value_ptr(matrix));
}
void Shader::SetUniform3fv(const std::string&& name, const glm::vec3& value) {

	glUniform3fv(GetUniformLocation(name.c_str()), 1, &value[0]);
}