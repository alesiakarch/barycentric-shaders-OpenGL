#version 410 core

layout(location = 0) out vec4 finalColour;

in vec2 uv;

uniform sampler2D UVTexture;
// t_i parameters, end goal maps that i expect to combine
uniform sampler2D DiffuseWeight;
uniform sampler2D ShadowWeight;
uniform sampler2D SpecularWeight;
uniform sampler2D OutlineWeight;


// control images to be assigned to each weight
uniform sampler2D diffuseTexture;
uniform sampler2D shadowTexture;
uniform sampler2D specularTexture;
uniform sampler2D outlineTexture;

void main()
{
    //finalColour = texture(UVTexture, uv);
    // trying to blend diffuse with shadow
    vec2 meshUV = clamp(texture(UVTexture, uv), 0.0, 1.0).rg; // get the uv coordinates

    float wx = texture(DiffuseWeight, uv).r; // weight for the first texture
    float wy = texture(ShadowWeight, uv).g; // weight for the second texture
    float wz = texture(SpecularWeight, uv).b; // weight for the third texture // my barycentric coords basically
    float ww = texture(OutlineWeight, uv).a; // weight for the fourth texture

    vec3 tex1 = texture(diffuseTexture, meshUV).rgb;
    vec3 tex2 = texture(shadowTexture, meshUV).rgb;
    vec3 tex3 = texture(specularTexture, meshUV).rgb;
    vec3 tex4 = texture(outlineTexture, meshUV).rgb;
     
    float totalWeight = wx + wy + wz + ww; // normalizing the weights to 1
    wx /= totalWeight;
    wy /= totalWeight;
    wz /= totalWeight;
    ww /= totalWeight;

    vec3 blendtex = tex1 * wx + tex2 * wy + tex3 * wz + tex4 * ww; // blending the textures
    finalColour = vec4(blendtex, 1.0);

    //finalColour = vec4(tex1, 1.0);
}