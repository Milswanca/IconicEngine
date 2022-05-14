#pragma once
#include <glad/glad.h>

#include "Assets/AssetManager.h"

class Texture : public AssetResource
{
public:
    enum TextureFormats
    {
        R8,
        RG8,
        RGB8,
        RGBA8,
        R16,
        RG16,
        RGB16,
        RGBA16,
        R32,
        RG32,
        RGB32,
        RGBA32,
    };

    struct TexFormatToGLType
    {
        TexFormatToGLType() :
            Format(GL_RGBA),
            DataType(GL_BYTE),
            Stride(32)
        {}
        
        TexFormatToGLType(GLenum InFormat, GLenum InDataType, unsigned int InStride) :
            Format(InFormat),
            DataType(InDataType),
            Stride(InStride)
        {}
        
        GLenum Format;
        GLenum DataType;
        unsigned int Stride;
    };

    struct TextureData
    {
        unsigned int Width;
        unsigned int Height;
        unsigned int Depth;

        TextureFormats TextureFormat;
        unsigned char* PixelData;
    };

public:
    Texture(Object* NewOuter);
    ~Texture();

    void Initialize(unsigned int W, unsigned int H, TextureFormats Format);
    void SetPixels(unsigned char* Pixels);
    virtual void UpdateResource(bool bGenerateMips);
    unsigned int GetTextureID() const;

private:
    unsigned int TextureID;
    TextureData* Data;

public:
    static std::map<TextureFormats, TexFormatToGLType> TexFormatsToGLTypes;
};