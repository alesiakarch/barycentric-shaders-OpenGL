#version 410 core

layout(location = 0) out vec4 finalColour;

in vec3 normal;
in vec2 uv;
in vec3 fragPos;
in vec3 weight; // each pass influences the weight of the texture


// texture maps that are going to be blended
uniform sampler2D diffuseTexture;
uniform sampler2D shadowTexture;
uniform sampler2D specularTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;

// calculate the light over the final maps?
void main()
{   

    // trying to blend diffuse with shadow

    float wx = weight.x; // weight for the first texture
    float wy = weight.y; // weight for the second texture
    float wz = weight.z; // weight for the third texture // my barycentric coords basically

    vec3 tex1 = texture(diffuseTexture, uv).rgb;
    vec3 tex2 = texture(shadowTexture, uv).rgb;
    vec3 tex3 = texture(specularTexture, uv).rgb;
     
    float totalWeight = wx + wy + wz; // normalizing the weights to 1
    wx /= totalWeight;
    wy /= totalWeight;
    wz /= totalWeight;

    vec3 blendtex = tex1 * wx + tex2 * wy + tex3 * wz; // blending the textures
    finalColour = vec4(blendtex, 1.0);
    //finalColour = vec4(uv.x, uv.y, 0.0, 1.0);
    //finalColour = vec4(texture(shadowTexture, uv).rgb, 1.0);
    
}