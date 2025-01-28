#version 410 core

layout(location = 0) out vec4 finalColour;

in vec3 normal;
in vec2 uv;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;


// calculate the specular highlights weights
// specular light calculation algorithm from: https://learnopengl.com/Lighting/Basic-Lighting
void main()
{   
    float specularStrength = 0.5;
    float shininess = 8.0;
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // calculate specular reflection
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float specular = specularStrength * spec; // will be multiplied with a texture later

    // specular weight to pass down to quad shader
    finalColour = vec4(0.0, 0.0, specular, 1.0);
}