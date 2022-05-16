#include "Texture.h"
#include <glad/glad.h>
#include <Core/Engine.h>

std::map<Texture::TextureFormats, Texture::TexFormatToGLType> Texture::TexFormatsToGLTypes
{
    {Texture::TextureFormats::RGBA8, Texture::TexFormatToGLType(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, sizeof(char) * 4)},   
    {Texture::TextureFormats::RGBA16, Texture::TexFormatToGLType(GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT, sizeof(short) * 4)},   
    {Texture::TextureFormats::RGBA32, Texture::TexFormatToGLType(GL_RGBA32F, GL_RGBA, GL_FLOAT, sizeof(float) * 4)},   
    {Texture::TextureFormats::RGB8, Texture::TexFormatToGLType(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, sizeof(char) * 3)},   
    {Texture::TextureFormats::RGB16, Texture::TexFormatToGLType(GL_RGB16F, GL_RGB, GL_HALF_FLOAT, sizeof(short) * 3)},   
    {Texture::TextureFormats::RGB32, Texture::TexFormatToGLType(GL_RGB32F, GL_RGB, GL_FLOAT, sizeof(float) * 3)},   
    {Texture::TextureFormats::RG8, Texture::TexFormatToGLType(GL_RG8, GL_RG, GL_UNSIGNED_BYTE, sizeof(char) * 2)},   
    {Texture::TextureFormats::RG16, Texture::TexFormatToGLType(GL_RG16F, GL_RG, GL_HALF_FLOAT, sizeof(short) * 2)},   
    {Texture::TextureFormats::RG32, Texture::TexFormatToGLType(GL_RG32F, GL_RG, GL_FLOAT, sizeof(float) * 2)},   
    {Texture::TextureFormats::R8, Texture::TexFormatToGLType(GL_R8, GL_RED, GL_UNSIGNED_BYTE, sizeof(char) * 1)},   
    {Texture::TextureFormats::R16, Texture::TexFormatToGLType(GL_R16F, GL_RED, GL_HALF_FLOAT, sizeof(short) * 1)},   
    {Texture::TextureFormats::R32, Texture::TexFormatToGLType(GL_R32F, GL_RED, GL_FLOAT, sizeof(float) * 1)},   
};

Texture* Texture::Create(Object* NewOuter, const CreateTextureParams& Params)
{
    Texture* Tex = Engine::Get()->CreateObject<Texture>(NewOuter);
    Tex->Initialize(Params.W, Params.H, Params.Format);

    if (Params.Pixels != nullptr)
    {
        Tex->SetPixels(Params.Pixels);
    }

    Tex->GenerateMips = Params.GenerateMips;
    Tex->UpdateResource();
    return Tex;
}

void Texture::Init()
{
    AssetResource::Init();

    GenerateMips = false;
    Data = new TextureData();
    Data->PixelData = nullptr;

    glGenTextures(1, &TextureID);
}

void Texture::Shutdown()
{
    AssetResource::Shutdown();

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

void Texture::UpdateResource()
{
    TexFormatToGLType GLData = TexFormatsToGLTypes[Data->TextureFormat];
    
    glBindTexture(GL_TEXTURE_2D, TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GLData.SourceFormat, Data->Width, Data->Height, 0, GLData.DesiredFormat, GLData.DataType, Data->PixelData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(GenerateMips)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

GLuint Texture::GetTextureID() const
{
    return TextureID;
}

void Texture::TextureBound(unsigned Index)
{
    
}

void Texture::TextureUnbound(unsigned Index)
{
    
}
