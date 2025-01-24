#version 410 core

layout(location = 0) out vec4 finalColour;

in vec3 normal;
in vec2 uv;
in vec3 baryCoords;
in vec4 fragPos;

// texture maps that are going to be blended
uniform sampler2D specularTexture1;

uniform vec3 lightPos;
uniform vec4 viewPos;


// calculate the light over the final maps?
void main()
{   
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos.xyz);
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(vec3(viewPos) - fragPos.xyz);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);  

    vec3 spectex = texture(specularTexture1, uv).rgb;
    //finalColour = vec4(specular, 1.0);
    finalColour = vec4(specular * spectex, 1.0);
    
    //finalColour = texture(specularTexture1, uv);
}