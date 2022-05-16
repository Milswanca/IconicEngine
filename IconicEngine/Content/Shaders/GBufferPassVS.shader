#version 410 core
layout(location = 0) in vec3 Position_VS_in;
layout(location = 1) in vec2 UV_VS_in;
layout(location = 2) in vec3 Normal_VS_in;
layout(location = 3) in vec3 Tangent_VS_in;
layout(location = 4) in vec3 Bitangent_VS_in;
layout(location = 5) in vec4 Color_VS_in;

out VS_OUT
{
    vec2 UV;
    vec4 Color;
    vec3 Position;
    vec3 Normal;
    mat3 TangentToWorldSpace;
} vs_out;

layout(std140) uniform Camera
{
    mat4 gViewProjection;
    vec3 gEyePosition;
};

uniform vec3 gLightPosition;
uniform mat4 gModel;

void main()
{
    vs_out.UV = UV_VS_in;
    vs_out.Color = Color_VS_in;
    vs_out.Position = (gModel * vec4(Position_VS_in, 1.0f)).xyz;
    vs_out.Normal = normalize(gModel * vec4(Normal_VS_in, 0.0f)).xyz;

    vec3 T = normalize(vec3(gModel * vec4(Tangent_VS_in, 0.0)));
    vec3 B = normalize(vec3(gModel * vec4(Bitangent_VS_in, 0.0)));
    vec3 N = normalize(vec3(gModel * vec4(Normal_VS_in, 0.0)));
    vs_out.TangentToWorldSpace = mat3(T, B, N);

    gl_Position = gViewProjection * vec4(vs_out.Position, 1.0);
}