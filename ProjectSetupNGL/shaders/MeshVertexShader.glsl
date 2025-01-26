#version 410 core

uniform mat4 MVP;
uniform mat4 modelMat;
uniform vec3 lightPos;
uniform vec3 viewPos;

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec2 vertUV;
layout(location = 3) in vec3 vertBaryCoords;


out vec3 normal;
out vec2 uv;
out vec3 baryCoords;
out vec3 fragPos;
out vec4 weight;

void main()
{
    gl_Position = MVP * vec4(vertPos,1.0);
    uv = vertUV; 
    baryCoords = vertBaryCoords;
    fragPos = vec3(modelMat * vec4(vertPos, 1.0));

    float reflectionFalloff = 2.0;
    // calculate diffuse light
    //normal = normalize(vertNormal);
    normal = normalize(mat3(transpose(inverse(modelMat))) * vertNormal);
    vec3 lightDir = normalize(lightPos - fragPos);  
    float lightIntensity = max(dot(lightDir, normal), 0.0); 
    
    // calculate specular light
    float specularStrength = 1.0;
    float shininess = 8.0;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0); 

    // Fresnel-based reflection falloff
    float fresnel = pow(1.0 - max(dot(viewDir, normal), 0.0), reflectionFalloff);

    weight = vec4(lightIntensity, 1 - lightIntensity, fresnel, spec); // how much light is on the fragment, assuming 2 textures
                                                                 // and satifying partition of unity
}