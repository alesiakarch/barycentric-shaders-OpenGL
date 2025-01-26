#version 410 core

uniform mat4 MVP;
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertColour;
out vec3 fragColour;
void main()
{
    fragColour = vertColour;
    gl_Position = MVP * vec4(vertPos,1.0);
}