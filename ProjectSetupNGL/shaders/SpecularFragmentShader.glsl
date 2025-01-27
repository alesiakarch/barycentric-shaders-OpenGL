#version 410 core

layout(location = 0) out vec4 finalColour;

in vec3 normal;
in vec2 uv;
in vec3 fragPos;

// texture maps that are going to be blended
uniform vec3 lightPos;
uniform vec3 viewPos;


// calculate the light over the final maps?
void main()
{   
    // calculate specular light
    float specularStrength = 1.0;
    float shininess = 8.0;
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    //vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0); 

    finalColour = vec4(0.0, 0.0, spec, 1.0);
}