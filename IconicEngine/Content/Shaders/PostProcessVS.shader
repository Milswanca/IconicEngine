#version 410 core
layout (location = 0) in vec3 Position_VS_in;
layout (location = 1) in vec3 UV_VS_in;

out VS_OUT
{
    vec3 UV;
} vs_out;

void main()
{
    vs_out.UV = UV_VS_in;
    gl_Position = vec4(Position_VS_in.x, Position_VS_in.y, 0.0f, 1.0f);
}