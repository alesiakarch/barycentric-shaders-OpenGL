#version 410 core

layout(location = 0) out vec4 finalColour;

in vec3 normal;
in vec2 uv;
in vec3 baryCoords;
in vec3 fragPos;
in vec4 weight;

// texture maps that are going to be blended
uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D diffuseTexture3;
uniform sampler2D diffuseTexture4;

uniform vec3 lightPos;
uniform vec3 viewPos;


// calculate the light over the final maps?
void main()
{   
    // trying to blend diffuse with shadow

    float wx = weight.x; // weight for the first texture
    float wy = weight.y; // weight for the second texture
    float wz = weight.z; // weight for the third texture // my barycentric coords basically
    float ww = weight.w; // weight for the fourth texture

    vec3 tex1 = texture(diffuseTexture1, uv).rgb;
    vec3 tex2 = texture(diffuseTexture2, uv).rgb;
    vec3 tex3 = texture(diffuseTexture3, uv).rgb;
    vec3 tex4 = texture(diffuseTexture4, uv).rgb;
     
    float totalWeight = wx + wy + wz + ww; // normalizing the weights to 1
    wx /= totalWeight;
    wy /= totalWeight;
    wz /= totalWeight;
    ww /= totalWeight;

    vec3 blendtex = tex1 * wx + tex2 * wy + tex3 * wz + tex4 * ww; // blending the textures
    finalColour = vec4(blendtex, 1.0);


//     // ambient light
//     float ambientStrength = 0.3;
//     vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0); // ambient light, strenght * light colour
    

//    // diffuse light
    // vec3 norm = normalize(normal);
    // vec3 lightDir = normalize(lightPos - fragPos);
    // float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = diff * vec3(1.0, 1.0, 1.0); // diffuse light, strenght * light colour 

//     vec3 diffuseResult = (ambient + diffuse) * blended_tex;
    
    //finalColour = vec4(diffuseResult, 1.0);
    //finalColour = texture(diffuseTexture1, uv); // just the texture
    //finalColour = vec4((lightDir * 0.5) + 0.5, 1.0);
    //finalColour = vec4((normalize(normal) * 0.5) + 0.5, 1.0);
    
}