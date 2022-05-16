#version 410 core

// define the number of CPs in the output patch
layout (vertices = 3) out;

uniform vec3 gEyeWorldPos;
uniform mat4 gViewProjection;

// attributes of the input CPs
in vec3 Position_CS_in[];
in vec3 UV_CS_in[];
in vec3 Normal_CS_in[];
in vec4 Color_CS_in[];

// attributes of the output CPs
out vec3 Position_ES_in[];
out vec3 UV_ES_in[];
out vec3 Normal_ES_in[];
out vec4 Color_ES_in[];

const float MIN_TES = 1;
const float MAX_TES = 4;

const float MIN_DIST = 1.0f;
const float MAX_DIST = 300.0f;

void main()
{
    // Set the control points of the output patch
    UV_ES_in[gl_InvocationID] = UV_CS_in[gl_InvocationID];
    Normal_ES_in[gl_InvocationID] = Normal_CS_in[gl_InvocationID];
    Position_ES_in[gl_InvocationID] = Position_CS_in[gl_InvocationID];
    Color_ES_in[gl_InvocationID] = Color_CS_in[gl_InvocationID];

    if(gl_InvocationID == 0)
    {
        vec3 center0 = Position_CS_in[0] + (Position_CS_in[1] - Position_CS_in[0]) / 2.0f;
        vec3 center1 = Position_CS_in[1] + (Position_CS_in[2] - Position_CS_in[1]) / 2.0f;
        vec3 center2 = Position_CS_in[2] + (Position_CS_in[0] - Position_CS_in[2]) / 2.0f;

        float dist0 = distance(gEyeWorldPos, center0);
        float dist1 = distance(gEyeWorldPos, center1);
        float dist2 = distance(gEyeWorldPos, center2);

        int tes0 = int(mix(MAX_TES, MIN_TES, clamp(dist0 / MAX_DIST, 0.0f, 1.0f)));
        int tes1 = int(mix(MAX_TES, MIN_TES, clamp(dist1 / MAX_DIST, 0.0f, 1.0f)));
        int tes2 = int(mix(MAX_TES, MIN_TES, clamp(dist2 / MAX_DIST, 0.0f, 1.0f)));

        float minDist = max(dist0, max(dist1, dist2));
        int tesInner = int(mix(MAX_TES, MIN_TES, clamp(minDist / MAX_DIST, 0.0f, 1.0f)));

        // Calculate the tessellation levels
        gl_TessLevelOuter[0] = tes0;
        gl_TessLevelOuter[1] = tes1;
        gl_TessLevelOuter[2] = tes2;
        gl_TessLevelInner[0] = tesInner;
    }
}