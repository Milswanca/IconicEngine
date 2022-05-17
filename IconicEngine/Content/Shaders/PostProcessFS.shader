#version 410 core
out vec4 Color_FS_out;

in VS_OUT
{
    vec3 UV;
} fs_in;

uniform float gInvert_Power;
uniform float gGreyscale_Power;
uniform sampler2D gTex_Scene;

void main()
{
    vec4 scene = texture(gTex_Scene, fs_in.UV.xy);

    // Invert
    vec4 PPInvertColor = vec4(1.0f - scene.xyz, 1.0f) * gInvert_Power;

    // Greyscale
    float average = 0.2126 * scene.r + 0.7152 * scene.g + 0.0722 * scene.b;
    vec4 PPGreyscale = vec4(average, average, average, 1.0f) * gGreyscale_Power;

    float BaseColorPower = max(1.0f - (gInvert_Power + gGreyscale_Power), 0.0f);

    vec4 FinalColor = vec4(scene.xyz * BaseColorPower, 1.0) + PPInvertColor + PPGreyscale;
    Color_FS_out = FinalColor;
}