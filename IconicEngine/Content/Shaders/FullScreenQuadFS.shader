#version 410 core
out vec4 Color_FS_out;

in VS_OUT
{
    vec3 Position;
    vec3 UV;
} fs_in;

uniform sampler2D gTex_Scene;
uniform float gInvert_Power;
uniform float gGreyscale_Power;

void main()
{
    vec4 BaseColor = texture(gTex_Scene, fs_in.UV.xy);

    // Invert
    vec4 PPInvertColor = vec4(1.0f - BaseColor.xyz, 1.0f) * gInvert_Power;

    // Greyscale
    float average = 0.2126 * BaseColor.r + 0.7152 * BaseColor.g + 0.0722 * BaseColor.b;
    vec4 PPGreyscale = vec4(average, average, average, 1.0f) * gGreyscale_Power;

    float BaseColorPower = max(1.0f - (gInvert_Power + gGreyscale_Power), 0.0f);

    Color_FS_out = vec4(BaseColor.xyz * BaseColorPower, 1.0) + PPInvertColor + PPGreyscale;
}