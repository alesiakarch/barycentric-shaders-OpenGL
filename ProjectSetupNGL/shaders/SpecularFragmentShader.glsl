#version 410 core

layout(location = 0) out vec4 finalColour;

in vec3 normal;
in vec2 uv;
in vec3 baryCoords;
in vec4 fragPos;

// texture maps that are going to be blended
uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D diffuseTexture3;

uniform vec3 lightPos;
uniform vec4 viewPos;


// calculate the light over the final maps?
void main()
{   
    // calculate barycentric weights
    vec3 tex1 = texture(diffuseTexture1, uv).rgb;
    vec3 tex2 = texture(diffuseTexture2, uv).rgb;
    vec3 tex3 = texture(diffuseTexture3, uv).rgb;
     
    //vec3 blended_tex = tex1 * baryCoords.x + tex2 * baryCoords.y + tex3 * baryCoords.z;
    vec3 blended_tex = texture(diffuseTexture1, uv).rgb; 

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(vec3(viewPos) - fragPos.xyz);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);  

    vec3 diffuseResult = (ambient + diffuse) * blended_tex;
    
    finalColour = vec4(diffuseResult, 1.0);
    //finalColour = vec4(baryCoords, 1.0);
    //finalColour = texture(diffuseTexture1, uv);
}