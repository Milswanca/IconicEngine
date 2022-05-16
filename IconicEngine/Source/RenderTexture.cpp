#include "RenderTexture.h"
#include <Core/Engine.h>

#include "RenderManager.h"

RenderTexture* RenderTexture::Create(Object* NewOuter, const CreateRenderTextureParams& Params)
{
    RenderTexture* Tex = Engine::Get()->CreateObject<RenderTexture>(NewOuter);
    Tex->Initialize(Params.W, Params.H, Params.Format);

    if (Params.Pixels)
    {
        Tex->SetPixels(Params.Pixels);
    }
    Tex->GenerateMips = Params.GenerateMips;

    Tex->AttachDepthBuffer();
    Tex->UpdateResource();
    return Tex;
}

void RenderTexture::Init()
{
    Texture::Init();
    
    glGenFramebuffers(1, &FBO);
}

void RenderTexture::Shutdown()
{
    Texture::Shutdown();
    
    glDeleteFramebuffers(1, &FBO);

    if (RBO)
        glDeleteRenderbuffers(1, &RBO);
}

void RenderTexture::Bind()
{
    GetRenderManager()->BindFramebuffer(this);
}

void RenderTexture::AttachDepthBuffer()
{
    if (RBO)
        return;

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Data->Width, Data->Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderTexture::UpdateResource()
{
    Texture::UpdateResource();

    RenderTexture* LastBound = GetRenderManager()->GetFramebuffer();
    
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GetTextureID(), 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    
    GetRenderManager()->BindFramebuffer(LastBound);
}

void RenderTexture::Clear(bool ClearDepth)
{
    RenderTexture* LastBound = GetRenderManager()->GetFramebuffer();
    
    Bind();
    GLenum Buffers = ClearDepth ? GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT : GL_COLOR_BUFFER_BIT;
    glClear(Buffers);
    
    GetRenderManager()->BindFramebuffer(LastBound);
}

unsigned RenderTexture::GetFBO() const
{
    return FBO;
}

unsigned RenderTexture::GetRBO() const
{
    return RBO;
}

void RenderTexture::FramebufferBound()
{
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
}

void RenderTexture::FramebufferUnbound()
{
    
}
