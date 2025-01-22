#version 410 core

uniform mat4 MVP;
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec2 vertUV;


out vec3 normal;
out vec2 uv;

void main()
{
    gl_Position = MVP * vec4(vertPos,1.0);
    uv = vertUV; 

}