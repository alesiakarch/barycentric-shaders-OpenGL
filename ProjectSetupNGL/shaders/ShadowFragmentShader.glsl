#version 410 core

layout(location = 0) out vec4 finalColour;


in vec3 normal;
in vec2 uv;
in vec3 baryCoords;
in vec4 fragPos;

// texture maps that are going to be blended
uniform sampler2D ShadowTexture1;
uniform vec3 lightPos;

// uniform vec4 viewPos;


// calculate the light over the final maps?
void main()
{   
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos.xyz);  
    float lightIntensity = max(dot(lightDir, normal), 0.0); // calculate how the normals align with the light
    vec3 diffuse = lightIntensity * vec3(5.0, 5.0, 5.0); // diffuse light, strenght * light colour 
    vec3 Shadowtex = texture(ShadowTexture1, uv).rgb;

    if (lightIntensity > 0) // if there is light on the fragment
    {
        finalColour = vec4(0.0 , 0.0, 0.0, 0.0); // black and/or empty
         
    }
    else
    {
        finalColour = texture(ShadowTexture1, uv); // just the texture
    }

    //finalColour = vec4(diffuse * Shadowtex, 1.0);
    //finalColour = vec4(fragPos * 0.1);
    //finalColour = vec4(lightPos, 1.0);

}