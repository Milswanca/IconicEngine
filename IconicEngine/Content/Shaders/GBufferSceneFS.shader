#version 410 core
layout(location = 0) out vec3 gAmbient;
layout(location = 1) out vec3 gAlbedo;
layout(location = 2) out vec3 gPosition;
layout(location = 3) out vec3 gNormal;
layout(location = 4) out vec3 gShineMatID;

in VS_OUT
{
    vec3 UV;
    vec4 Color;
    vec3 Position;
    vec3 Normal;
    mat3 TangentToWorldSpace;
} fs_in;

layout(std140) uniform Camera
{
    mat4 gViewProjection;
    vec4 gEyePosition;
    float gNear;
    float gFar;
};

// Diffuse
uniform vec3 gDiffuseColor;
uniform sampler2D gTex_Diffuse;
uniform float gTex_Diffuse_Power;

// Normal
uniform sampler2D gTex_Normals;
uniform float gTex_Normals_Power;

// Specular
uniform float gShininess;
uniform float gShininessStrength;
uniform sampler2D gTex_SpecularMask;
uniform float gTex_SpecularMask_Power;

// Uniforms
uniform int gMaterialID;
uniform vec3 gAmbientColor;

float select(float a, float b, float chooseB)
{
    return ((1.0f - chooseB) * a) + (chooseB * b);
}

vec3 select(vec3 a, vec3 b, float chooseB)
{
    return ((1.0f - chooseB) * a) + (chooseB * b);
}

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return ((2.0 * gNear * gFar) / (gFar + gNear - z * (gFar - gNear))) / gFar;
}

void main()
{
    // Ambient Light
    gAmbient = gAmbientColor * 0.2f;

    // Albedo
    gAlbedo.xyz = select(gDiffuseColor, texture(gTex_Diffuse, fs_in.UV.xy).xyz, gTex_Diffuse_Power);

    // Specular
    gShineMatID.r = gShininessStrength * select(1.0f, texture(gTex_SpecularMask, fs_in.UV.xy).x, gTex_SpecularMask_Power);
    gShineMatID.g = gShininess;
    gShineMatID.b = gMaterialID;

    // Position
    gPosition = fs_in.Position;

    // Normals
    gNormal = select(fs_in.Normal, fs_in.TangentToWorldSpace * normalize(texture(gTex_Normals, fs_in.UV.xy).xyz * 2.0 - 1.0), gTex_Normals_Power);

    gl_FragDepth = LinearizeDepth(gl_FragCoord.z);
}