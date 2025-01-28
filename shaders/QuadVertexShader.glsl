#version 410 core

uniform mat4 MVP;
layout(location = 0) in vec2 vertPos;
layout(location = 1) in vec2 vertUV;

out vec2 uv;

// processing the quad data to be used in the quad shader
void main()
{
    gl_Position = vec4(vertPos, 0.0, 1.0);
    uv = vertUV;
}