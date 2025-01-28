#version 410 core

layout(location = 0) out vec4 finalColour;

in vec3 normal;
in vec2 uv;
in vec3 fragPos;

uniform vec3 lightPos;

// calculates the amount of light that hits the fragment, it will become diffuse weight
// diffuse light calculation algorithm from: https://learnopengl.com/Lighting/Basic-Lighting 
void main()
{   
    //vec3 lightColour = vec3(1.0, 1.0, 1.0);

    // ambient light
    //float ambientStrength = 0.3;
    //vec3 ambient = ambientStrength * lightColour;

    // diffuse light
    vec3 lightDir = normalize(lightPos - fragPos);
    float lightIntensity = max(dot(normal, lightDir), 0.0);

    // stores the weight to pass down to quad shader
    finalColour = vec4(lightIntensity, 0.0, 0.0, 1.0);
    
}