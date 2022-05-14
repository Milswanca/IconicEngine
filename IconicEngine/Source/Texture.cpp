#include "Texture.h"
#include <glad/glad.h>

std::map<Texture::TextureFormats, Texture::TexFormatToGLType> Texture::TexFormatsToGLTypes
{
    {Texture::TextureFormats::RGBA8, Texture::TexFormatToGLType(GL_RGBA, GL_UNSIGNED_BYTE, sizeof(char) * 4)},   
    {Texture::TextureFormats::RGBA16, Texture::TexFormatToGLType(GL_RGBA, GL_HALF_FLOAT, sizeof(short) * 4)},   
    {Texture::TextureFormats::RGBA32, Texture::TexFormatToGLType(GL_RGBA, GL_FLOAT, sizeof(float) * 4)},   
    {Texture::TextureFormats::RGB8, Texture::TexFormatToGLType(GL_RGB, GL_UNSIGNED_BYTE, sizeof(char) * 3)},   
    {Texture::TextureFormats::RGB16, Texture::TexFormatToGLType(GL_RGB, GL_HALF_FLOAT, sizeof(short) * 3)},   
    {Texture::TextureFormats::RGB32, Texture::TexFormatToGLType(GL_RGB, GL_FLOAT, sizeof(float) * 3)},   
    {Texture::TextureFormats::RG8, Texture::TexFormatToGLType(GL_RG, GL_UNSIGNED_BYTE, sizeof(char) * 2)},   
    {Texture::TextureFormats::RG16, Texture::TexFormatToGLType(GL_RG, GL_HALF_FLOAT, sizeof(short) * 2)},   
    {Texture::TextureFormats::RG32, Texture::TexFormatToGLType(GL_RG, GL_FLOAT, sizeof(float) * 2)},   
    {Texture::TextureFormats::R8, Texture::TexFormatToGLType(GL_RED, GL_UNSIGNED_BYTE, sizeof(char) * 1)},   
    {Texture::TextureFormats::R16, Texture::TexFormatToGLType(GL_RED, GL_HALF_FLOAT, sizeof(short) * 1)},   
    {Texture::TextureFormats::R32, Texture::TexFormatToGLType(GL_RED, GL_FLOAT, sizeof(float) * 1)},   
};

Texture::Texture(Object* NewOuter) : AssetResource(NewOuter)
{
    Data = new TextureData();
    Data->PixelData = nullptr;

    glGenTextures(1, &TextureID);
}

Texture::~Texture()
{
    if(Data->PixelData != nullptr)
    {
        delete[] Data->PixelData;
        Data->PixelData = nullptr;
    }
    
    delete Data;

    glDeleteTextures(1, &TextureID);
}

void Texture::Initialize(unsigned W, unsigned H, TextureFormats Format)
{
    Data->Width = W;
    Data->Height = H;
    Data->TextureFormat = Format;
}

void Texture::SetPixels(unsigned char* Pixels)
{
    if(Data->PixelData != nullptr)
    {
        delete[] Data->PixelData;
        Data->PixelData = nullptr;
    }
    
    unsigned int NumBytes = Data->Width * Data->Height * TexFormatsToGLTypes[Data->TextureFormat].Stride;
    Data->PixelData = new unsigned char[NumBytes];
    memcpy(Data->PixelData, Pixels, NumBytes);
}

void Texture::UpdateResource(bool bGenerateMips)
{
    TexFormatToGLType GLData = TexFormatsToGLTypes[Data->TextureFormat];
    
    glBindTexture(GL_TEXTURE_2D, TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GLData.Format, Data->Width, Data->Height, 0, GLData.Format, GLData.DataType, Data->PixelData);

    if(bGenerateMips)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

unsigned Texture::GetTextureID() const
{
    return TextureID;
}
