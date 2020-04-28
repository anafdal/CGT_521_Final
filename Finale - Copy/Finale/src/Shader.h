#pragma once
//modified from Cherno
#include <iostream>
#include<unordered_map>//hash map
#include "glm/glm.hpp"


class Shader {

private:
	
	unsigned int shaderId;
	//caching for uniforms; only retrieve stuff the first time
	std::unordered_map<std::string, int> locationCache;

public:
	Shader(const std::string& filename1, const std::string& filename2);
	~Shader();//destructor

	void Bind() const;
	void Unbind() const;

	//set uniforms
	void SetUniform1i(const std::string&& name, int i);
	void SetUniform1f(const std::string&& name, float f);
	void SetUniform4f(const std::string&& name, float f0, float f1, float f2, float f3);
	void SetUniformMatrix4fv(const std::string&& name, const glm::mat4& matrix);
	void SetUniform3fv(const std::string&& name, const glm::vec3& value);
private:
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(const std::string source, unsigned int type);
	const std::string ParseShader1(const std::string& filepath);
	int GetUniformLocation(const std::string&& name);

};
