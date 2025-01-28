#version 410 core

layout(location = 0) out vec4 finalColour;

in vec3 normal;
in vec2 uv;
in vec3 fragPos;

uniform vec3 lightPos;

// uses inverted light intensity to calculate the shadow
void main()
{   
    vec3 lightDir = normalize(lightPos - fragPos);  
    float lightIntensity = max(dot(lightDir, normal), 0.0); 

    // shadow weight to pass down to quad shader
    finalColour = vec4(0.0, 1 - lightIntensity, 0.0, 1.0);

}