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
            SourceFormat(GL_RGBA8),
            DesiredFormat(GL_RGBA),
            DataType(GL_BYTE),
            Stride(32)
        {}
        
        TexFormatToGLType(GLenum InSourceFormat, GLenum InDesiredFormat, GLenum InDataType, unsigned int InStride) :
            SourceFormat(InSourceFormat),
            DesiredFormat(InDesiredFormat),
            DataType(InDataType),
            Stride(InStride)
        {}
        
        GLenum SourceFormat;
        GLenum DesiredFormat;
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

    struct CreateTextureParams
    {
        unsigned int W;
        unsigned int H;
        unsigned char* Pixels;
        TextureFormats Format;
        bool GenerateMips;
    };

public:
    static Texture* Create(Object* NewOuter, const CreateTextureParams& Params);

    IMPLEMENT_CONSTRUCTOR(Texture, AssetResource);

    virtual void Init() override;
    virtual void Shutdown() override;

    void Initialize(unsigned int W, unsigned int H, TextureFormats Format);
    void SetPixels(unsigned char* Pixels);
    virtual void UpdateResource();
    GLuint GetTextureID() const;

protected:
    virtual void TextureBound(unsigned int Index);
    virtual void TextureUnbound(unsigned int Index);
    
    bool GenerateMips;
    GLuint TextureID;
    TextureData* Data;

private:
    static std::map<TextureFormats, TexFormatToGLType> TexFormatsToGLTypes;
    friend class RenderManager;
};