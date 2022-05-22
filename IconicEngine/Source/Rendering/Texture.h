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
        Depth
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

public:
    IMPLEMENT_CONSTRUCTOR(Texture, AssetResource);

    virtual void Bind(unsigned int Index);

    virtual void UpdateResource() = 0;
    virtual GLuint GetTextureID() const = 0;

protected:
    virtual void TextureBound(unsigned int Index);
    virtual void TextureUnbound(unsigned int Index);
    
    bool bGenerateMips = true;
    static std::map<TextureFormats, TexFormatToGLType> TexFormatsToGLTypes;

private:
    friend class RenderManager;
};