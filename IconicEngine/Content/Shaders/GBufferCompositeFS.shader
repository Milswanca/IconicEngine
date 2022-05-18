#version 410 core
out vec4 Color_FS_out;

in VS_OUT
{
    vec3 UV;
} fs_in;

uniform sampler2D gTex_Position;
uniform sampler2D gTex_Normal;
uniform sampler2D gTex_Albedo;
uniform sampler2D gTex_Ambient;
uniform sampler2D gTex_Shine;

uniform vec3 gLightPosition;

layout(std140) uniform Camera
{
    mat4 gViewProjection;
    vec3 gEyePosition;
};

void main()
{
    vec3 Position = texture(gTex_Position, fs_in.UV.xy).rgb;
    vec3 Normal = texture(gTex_Normal, fs_in.UV.xy).rgb;
    vec3 Albedo = texture(gTex_Albedo, fs_in.UV.xy).rgb;
    vec3 Ambient = texture(gTex_Ambient, fs_in.UV.xy).rgb;
    float Spec = texture(gTex_Shine, fs_in.UV.xy).r;
    float Shininess = texture(gTex_Shine, fs_in.UV.xy).g;
    float ShininessStrength = texture(gTex_Shine, fs_in.UV.xy).b;

    vec3 viewDir = normalize(gEyePosition - Position);

    // Light Precalc
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightDir = normalize(gLightPosition - Position);
    float diff = max(dot(Normal, lightDir), 0.0f);
    vec3 reflectDir = reflect(-lightDir, Normal);

    // Diffuse Light
    vec3 diffuse = diff * lightColor;

    // Specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess) * ShininessStrength * Spec;
    vec3 specular = spec * lightColor;

    // Result
    vec3 result = (Ambient + diffuse + specular) * Albedo;
    Color_FS_out = vec4(result, 1.0f);
}