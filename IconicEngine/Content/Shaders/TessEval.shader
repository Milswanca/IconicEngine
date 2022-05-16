#version 410 core

layout(triangles, equal_spacing, ccw) in;

uniform mat4 gViewProjection;
uniform sampler2D gTex_BumpMap;
uniform float gTex_BumpMap_Power;

in vec3 Position_ES_in[];
in vec3 UV_ES_in[];
in vec3 Normal_ES_in[];
in vec4 Color_ES_in[];

out vec3 Position_FS_in;
out vec3 UV_FS_in;
out vec3 Normal_FS_in;
out vec4 Color_FS_in;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

vec4 interpolate4D(vec4 v0, vec4 v1, vec4 v2)
{
    return vec4(gl_TessCoord.x) * v0 + vec4(gl_TessCoord.y) * v1 + vec4(gl_TessCoord.z) * v2;
}

void main()
{
    // Interpolate the attributes of the output vertex using the barycentric coordinates
    UV_FS_in = interpolate3D(UV_ES_in[0], UV_ES_in[1], UV_ES_in[2]);
    Normal_FS_in = interpolate3D(Normal_ES_in[0], Normal_ES_in[1], Normal_ES_in[2]);
    Normal_FS_in = normalize(Normal_FS_in);
    Position_FS_in = interpolate3D(Position_ES_in[0], Position_ES_in[1], Position_ES_in[2]);
    Color_FS_in = interpolate4D(Color_ES_in[0], Color_ES_in[1], Color_ES_in[2]);

    // Displace the vertex along the normal
    float Displacement = texture(gTex_BumpMap, UV_FS_in.xy).x * 5.0f;
    Position_FS_in += Normal_FS_in * Displacement * gTex_BumpMap_Power;
    gl_Position = gViewProjection * vec4(Position_FS_in, 1.0);
}