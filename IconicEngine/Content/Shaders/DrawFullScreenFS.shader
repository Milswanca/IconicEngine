#version 410 core
out vec4 Color_FS_out;

in VS_OUT
{
    vec3 UV;
} fs_in;

uniform sampler2D gTex_Output;

void main()
{
    Color_FS_out = texture(gTex_Output, fs_in.UV.xy);
}
