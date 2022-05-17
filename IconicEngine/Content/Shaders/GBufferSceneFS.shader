#version 410 core
layout(location = 0) out vec3 gAmbient;
layout(location = 1) out vec3 gAlbedo;
layout(location = 2) out vec3 gPosition;
layout(location = 3) out vec3 gNormal;
layout(location = 4) out vec3 gShine;

in VS_OUT
{
    vec3 UV;
    vec4 Color;
    vec3 Position;
    vec3 Normal;
    mat3 TangentToWorldSpace;
} fs_in;

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
uniform vec3 gAmbientColor;

float select(float a, float b, float chooseB)
{
    return ((1.0f - chooseB) * a) + (chooseB * b);
}

vec3 select(vec3 a, vec3 b, float chooseB)
{
    return ((1.0f - chooseB) * a) + (chooseB * b);
}

void main()
{
    // Ambient Light
    gAmbient = gAmbientColor * 0.2f;

    // Albedo
    gAlbedo.xyz = select(gDiffuseColor, texture(gTex_Diffuse, fs_in.UV.xy).xyz, gTex_Diffuse_Power);

    // Specular
    gShine.r = select(1.0f, texture(gTex_SpecularMask, fs_in.UV.xy).x, gTex_SpecularMask_Power);
    gShine.g = gShininess;
    gShine.b = gShininessStrength;

    // Position
    gPosition = fs_in.Position;

    // Normals
    gNormal = select(fs_in.Normal, fs_in.TangentToWorldSpace * normalize(texture(gTex_Normals.xy, fs_in.UV.xy).xyz * 2.0 - 1.0), gTex_Normals_Power);
}