#include "Texture.h"
#include <glad/glad.h>
#include <Core/Engine.h>
#include "RenderManager.h"

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

void Texture::Bind(unsigned int Index)
{
    GetRenderManager()->BindTexture(this, Index);
}

void Texture::UpdateResource()
{

}

GLuint Texture::GetTextureID() const
{
    return -1;
}

void Texture::TextureBound(unsigned Index)
{
    
}

void Texture::TextureUnbound(unsigned Index)
{
    
}
