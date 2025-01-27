#version 410 core

layout(location = 0) out vec4 finalColour;

in vec3 normal;
in vec2 uv;
in vec3 fragPos;


// texture maps that are going to be blended
uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D diffuseTexture3;

uniform vec3 lightPos;

// calculate the light over the final maps?
void main()
{   
    //vec3 lightColour = vec3(1.0, 1.0, 1.0);

    // ambient light
    //float ambientStrength = 0.3;
    //vec3 ambient = ambientStrength * lightColour; // ambient light, strenght * light colour
    

   // diffuse light
    vec3 lightDir = normalize(lightPos - fragPos);
    float lightIntensity = max(dot(normal, lightDir), 0.0);
    //vec3 diffuse = lightIntensity * lightColour; // diffuse light, strenght * light colour 

      
    finalColour = vec4(lightIntensity, 0.0, 0.0, 1.0);
    
}