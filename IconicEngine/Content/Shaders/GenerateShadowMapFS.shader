#version 410 core

layout(std140) uniform Camera
{
    mat4 gViewProjection;
    vec4 gEyePosition;
    float gNear;
    float gFar;
};

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return ((2.0 * gNear * gFar) / (gFar + gNear - z * (gFar - gNear))) / gFar;
}

void main()
{
    gl_FragDepth = LinearizeDepth(gl_FragCoord.z);
}