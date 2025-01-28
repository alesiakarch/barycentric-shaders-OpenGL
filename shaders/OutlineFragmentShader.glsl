#version 410 core

layout(location = 0) out vec4 finalColour;

in vec3 normal;
in vec2 uv;
in vec3 fragPos;

uniform vec3 lightPos;

// edge detection using mesh normals + varying thickness depending on the light intensity
void main()
{   
    vec3 lightDir = normalize(lightPos - fragPos);
    float lightIntensity = max(dot(lightDir, normal), 0.0f);
    float outline = abs(normal.x) + abs(normal.y) + abs(normal.z); // edge detection
    float thinkness = 0.1f + (1.0f - lightIntensity) * 0.5f;

    float outlineMask = outline * thinkness;
    // outline weight to pass down to quad shader
    finalColour = vec4(0.0f, 0.0f, 0.0f, outlineMask);
}