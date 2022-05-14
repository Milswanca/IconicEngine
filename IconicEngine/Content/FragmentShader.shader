#version 410 core
out vec4 Color_FS_out;

in VS_OUT
{
    vec3 UV;
    vec4 Color;
    vec3 Position;
    vec3 Normal;

    vec3 Normal_TS;
    vec3 Position_TS;
    vec3 EyePosition_TS;
    vec3 LightPosition_TS;
} fs_in;

// Diffuse
uniform vec3 gDiffuseColor;
uniform float gTex_Diffuse_Power;
uniform sampler2D gTex_Diffuse;

// Normal
uniform float gTex_Normals_Power;
uniform sampler2D gTex_Normals;

// Specular
uniform float gShininess;
uniform float gShininessStrength;
uniform float gTex_SpecularMask_Power;
uniform sampler2D gTex_SpecularMask;

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
    vec3 viewDir = normalize(fs_in.EyePosition_TS - fs_in.Position_TS);
    
    // Normals
    vec3 normals = select(fs_in.Normal_TS, normalize(texture(gTex_Normals, fs_in.UV.xy).xyz * 2.0 - 1.0), gTex_Normals_Power);
    
    // Light Precalc
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightDir = normalize(fs_in.LightPosition_TS - fs_in.Position_TS);  
    float diff = max(dot(normals, lightDir), 0.0f);
    vec3 reflectDir = reflect(-lightDir, normals);
    
    // Albedo
    vec3 albedo = select(gDiffuseColor, texture(gTex_Diffuse, fs_in.UV.xy).xyz, gTex_Diffuse_Power);
    
    // Ambient Light
    vec3 ambient = gAmbientColor * 0.2f;
    
    // Diffuse Light
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specMask = select(1.0f, texture(gTex_SpecularMask, fs_in.UV.xy).x, gTex_SpecularMask_Power);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), gShininess) * gShininessStrength * specMask;
    vec3 specular = spec * lightColor;  
    
    // Result
    vec3 result = (ambient + diffuse + specular) * albedo;
    Color_FS_out = vec4(result, 1.0f);
}