#version 400 core


layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
uniform int pass;

out vec2 TexCoords;

void main()
{
if(pass==2){

    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 

    }
}