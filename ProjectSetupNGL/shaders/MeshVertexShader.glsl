#version 410 core

uniform mat4 MVP;
uniform mat4 modelMat;
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec2 vertUV;
layout(location = 3) in vec3 vertBaryCoords;


out vec3 normal;
out vec2 uv;
out vec3 baryCoords;
out vec4 fragPos;

void main()
{
    gl_Position = MVP * vec4(vertPos,1.0);
    uv = vertUV; 
    baryCoords = vertBaryCoords;
    normal = vertNormal;
    fragPos = modelMat * vec4(vertPos, 1.0);
}