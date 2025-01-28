#version 410 core

layout(location = 0) out vec4 finalColour;

in vec2 uv; // quad uv coordinates

// uvs of the loaded mesh
uniform sampler2D UVTexture;

// t_i parameters, that provide the weights for the textures
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
    // get mehs uv coordinates
    vec2 meshUV = texture(UVTexture, uv).rg; 
    meshUV = clamp(meshUV, 0.0f, 1.0f); 

    // extract the weights from the textures
    float wx = texture(DiffuseWeight, uv).r; // weight for the first texture
    float wy = texture(ShadowWeight, uv).g; // weight for the second texture
    float wz = texture(SpecularWeight, uv).b; // weight for the third texture // my barycentric coords basically
    float ww = texture(OutlineWeight, uv).a; // weight for the fourth texture

    // extract the textures from the control images, wrapped to the mesh uvs
    vec3 tex1 = texture(diffuseTexture, meshUV).rgb;
    vec3 tex2 = texture(shadowTexture, meshUV).rgb;
    vec3 tex3 = texture(specularTexture, meshUV).rgb;
    vec3 tex4 = texture(outlineTexture, meshUV).rgb;
     
    float totalWeight = wx + wy + wz + ww; // ensure the weights satisfy the partition of unity,
    wx /= totalWeight;                     // those are essentially our barycentric coordinates at each vertex,                         
    wy /= totalWeight;                     // enabling texture blending
    wz /= totalWeight;
    ww /= totalWeight;

    // texture blending based on parameters weight across the surface
    vec3 blendtex = tex1 * wx + tex2 * wy + tex3 * wz + tex4 * ww; // blending the textures
    
    // output the final colour
    finalColour = vec4(blendtex, 1.0f);
}