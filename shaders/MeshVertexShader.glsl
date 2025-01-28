#version 410 core

uniform mat4 MVP;
uniform mat4 modelMat;

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec2 vertUV;

out vec3 normal;
out vec2 uv;
out vec3 fragPos;

// processing the mesh data to be used in the parameters shaders
void main()
{
    gl_Position = MVP * vec4(vertPos,1.0f);
    uv = vertUV; 
    fragPos = vec3(modelMat * vec4(vertPos, 1.0f));
    normal = normalize(mat3(transpose(inverse(modelMat))) * vertNormal); // tranfsormed from object space to world space
}