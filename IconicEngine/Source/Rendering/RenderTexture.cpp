#include "RenderTexture.h"
#include "RenderManager.h"

void RenderTexture::Init()
{
    Texture::Init();
    
    glGenFramebuffers(1, &FBO);
}

void RenderTexture::Shutdown()
{
    Texture::Shutdown();
    
    glDeleteFramebuffers(1, &FBO);
}

void RenderTexture::BindFramebuffer()
{
    GetRenderManager()->BindFramebuffer(this);
}

void RenderTexture::Clear(bool ClearDepth)
{
    RenderTexture* LastBound = GetRenderManager()->GetFramebuffer();
    
    BindFramebuffer();
    GLenum Buffers = ClearDepth ? GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT : GL_COLOR_BUFFER_BIT;
    glClear(Buffers);
    
    GetRenderManager()->BindFramebuffer(LastBound);
}

unsigned RenderTexture::GetFBO() const
{
    return FBO;
}

void RenderTexture::FramebufferBound()
{

}

void RenderTexture::FramebufferUnbound()
{
    
}
