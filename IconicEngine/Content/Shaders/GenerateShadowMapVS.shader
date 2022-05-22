#version 410 core
layout(location = 0) in vec3 Position_VS_in;

uniform mat4 gModel;

layout(std140) uniform Camera
{
    mat4 gViewProjection;
    vec3 gEyePosition;
};

void main()
{
    gl_Position = gViewProjection * gModel * vec4(Position_VS_in, 1.0f);
}