#version 410 core
out vec4 Color_FS_out;

in VS_OUT
{
    vec3 UV;
} fs_in;

struct FragmentData
{
    vec3 Position;
    vec3 Normal;
    vec3 Albedo;
    vec3 Ambient;
    float Specular;
    float Shininess;
    int MaterialID;
};

struct PointLight
{
    vec4 Position;
    vec4 LightColor;
    float Intensity;
    float Radius;
};

struct DirectionalLight
{
    vec4 Direction;
    vec4 LightColor;
    float Intensity;
};

layout(std140) uniform Camera
{
    mat4 gViewProjection;
    vec4 gEyePosition;
    float gNear;
    float gFar;
};

#define MAX_POINT_LIGHTS 500
#define MAX_DIRECTIONAL_LIGHTS 8
layout(std140) uniform Lights
{
    PointLight PointLights[MAX_POINT_LIGHTS];
    DirectionalLight DirectionalLights[MAX_DIRECTIONAL_LIGHTS];
    
    int NumPointLights;
    int NumDirectionalLights;
};

uniform sampler2D gTex_Position;
uniform sampler2D gTex_Normal;
uniform sampler2D gTex_Albedo;
uniform sampler2D gTex_Ambient;
uniform sampler2D gTex_ShineMatID;

uniform vec3 gLightPosition;

float GetAttenuation(PointLight _light, vec3 _fragPos)
{
    // attenuation
    float distance = length(_light.Position.xyz - _fragPos);
    float attenuation = clamp(1.0 - distance * distance / (_light.Radius * _light.Radius), 0.0, 1.0);
    return attenuation;
}

vec3 ProcessUnlit(PointLight _light, FragmentData _fragment, vec3 _result)
{
    return _fragment.Albedo;
}

vec3 ProcessPhong(PointLight _light, FragmentData _fragment, vec3 _result)
{
    vec3 viewDir = normalize(gEyePosition.xyz - _fragment.Position);
    vec3 lightDir = normalize(_light.Position.xyz - _fragment.Position);

    // diffuse shading
    float diff = max(dot(_fragment.Normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, _fragment.Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), _fragment.Shininess) * _fragment.Specular;

    float attenuation = GetAttenuation(_light, _fragment.Position);
    vec3 diffuse = _fragment.Albedo * diff * attenuation * _light.LightColor.rgb;
    vec3 specular = _fragment.Specular * spec * attenuation * _light.LightColor.rgb;
    return _result + (diffuse + specular) * _light.Intensity;
}

vec3 ProcessPointLight(PointLight _light, FragmentData _fragment, vec3 _result)
{
    switch (_fragment.MaterialID)
    {
    case 0:
        return ProcessUnlit(_light, _fragment, _result);
    case 1:
        return ProcessPhong(_light, _fragment, _result);
    }
}

vec3 ProcessUnlit(DirectionalLight _light, FragmentData _fragment, vec3 _result)
{
    return _fragment.Albedo;
}

vec3 ProcessPhong(DirectionalLight _light, FragmentData _fragment, vec3 _result)
{
    vec3 viewDir = normalize(gEyePosition.xyz - _fragment.Position);
    vec3 lightDir = _light.Direction.xyz;

    // diffuse shading
    float diff = max(dot(_fragment.Normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, _fragment.Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), _fragment.Shininess) * _fragment.Specular;

    vec3 diffuse = _fragment.Albedo * diff * _light.LightColor.rgb;
    vec3 specular = _fragment.Specular * spec * _light.LightColor.rgb;
    return _result + (diffuse + specular) * _light.Intensity;
}

vec3 ProcessDirectionalLight(DirectionalLight _light, FragmentData _fragment, vec3 _result)
{
    switch (_fragment.MaterialID)
    {
    case 0:
        return ProcessUnlit(_light, _fragment, _result);
    case 1:
        return ProcessPhong(_light, _fragment, _result);
    }
}

void main()
{
    vec3 SpecMatIDSample = texture(gTex_ShineMatID, fs_in.UV.xy).rgb;
    
    FragmentData frag;
    frag.Position = texture(gTex_Position, fs_in.UV.xy).rgb;
    frag.Normal = texture(gTex_Normal, fs_in.UV.xy).rgb;
    frag.Albedo = texture(gTex_Albedo, fs_in.UV.xy).rgb;
    frag.Ambient = texture(gTex_Ambient, fs_in.UV.xy).rgb;
    frag.Specular = SpecMatIDSample.r;
    frag.Shininess = SpecMatIDSample.g;
    frag.MaterialID = int(SpecMatIDSample.b);

    vec3 result = vec3(0.0f);
    for (int i = 0; i < NumPointLights; ++i)
    {
        result = ProcessPointLight(PointLights[i], frag, result);
    }

    for (int i = 0; i < NumDirectionalLights; ++i)
    {
        result = ProcessDirectionalLight(DirectionalLights[i], frag, result);
    }

    result = clamp(result, 0, 1);

    // Result
    Color_FS_out = vec4(result, 1.0f);
}