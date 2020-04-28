#version 400 core

//match with glAttribPointer 0 index
//match with glAttribPointer 1 index


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform int pass;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


//out vec3 ourColor;
//out vec2 TexCoord;
out vec3 TexCoord;


void main()
{
 if(pass==1){
    TexCoord = aPos;
    vec4 pos = projection * view *model*vec4(aPos, 1.0);
	gl_Position = pos.xyww;
	//ourColor = aColor;
	}

}