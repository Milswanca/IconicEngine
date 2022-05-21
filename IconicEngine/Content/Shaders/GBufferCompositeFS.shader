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
    float Padding;
};

struct PointLight
{
    vec4 Position;
    vec4 LightColor;
    float Radius;
};

#define MAX_POINT_LIGHTS 8
layout(std140) uniform Lights
{
    PointLight PointLights[MAX_POINT_LIGHTS];
    int NumPointLights;
};

float GetAttenuation(PointLight _light, vec3 _fragPos)
{
    // attenuation
    float distance = length(_light.Position.xyz - _fragPos);
    float attenuation = clamp(1.0 - distance * distance / (_light.Radius * _light.Radius), 0.0, 1.0);
    return attenuation;
}

void main()
{
    vec3 Position = texture(gTex_Position, fs_in.UV.xy).rgb;
    vec3 Normal = texture(gTex_Normal, fs_in.UV.xy).rgb;
    vec3 Albedo = texture(gTex_Albedo, fs_in.UV.xy).rgb;
    vec3 Ambient = texture(gTex_Ambient, fs_in.UV.xy).rgb;
    float Specular = texture(gTex_Shine, fs_in.UV.xy).r;
    float Shininess = texture(gTex_Shine, fs_in.UV.xy).g;

    vec3 result = vec3(0.0f);
    for (int i = 0; i < NumPointLights; ++i)
    {
        vec3 viewDir = normalize(gEyePosition - Position);
        vec3 lightDir = normalize(PointLights[i].Position.xyz - Position);

        // diffuse shading
        float diff = max(dot(Normal, lightDir), 0.0);

        // specular shading
        vec3 reflectDir = reflect(-lightDir, Normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess) * Specular;

        float attenuation = GetAttenuation(PointLights[i], Position);

        vec3 ambient = Ambient * attenuation * PointLights[i].LightColor.rgb;
        vec3 diffuse = Albedo * diff * attenuation * PointLights[i].LightColor.rgb;
        vec3 specular = Specular * spec* attenuation * PointLights[i].LightColor.rgb;
        result += ambient + diffuse + specular;
    }

    // Result
    Color_FS_out = vec4(result, 1.0f);
}