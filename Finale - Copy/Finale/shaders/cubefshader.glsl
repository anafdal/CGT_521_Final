#version 400 core
//modofied from OpenGLLearn

out vec4 FragColor;

in vec3 normal;
in vec3 Pos;

uniform vec3 cameraPos;
uniform samplerCube cubemap;

void main()
{    
    vec3 N = normalize(Pos - cameraPos);
    vec3 Reflect = reflect(N, normalize(normal));
    FragColor = vec4(texture(cubemap, Reflect).rgb, 1.0);
}