#version 400 core
//modofied from OpenGLLearn

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 Pos;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
    normal = mat3(transpose(inverse(m))) * aNormal;
    Pos= vec3(m* vec4(aPos, 1.0));
    gl_Position = p * v* m *vec4(aPos, 1.0);
}