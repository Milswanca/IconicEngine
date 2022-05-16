#include "RenderManager.h"

#include "CameraComponent.h"
#include "IDrawable.h"
#include "Material.h"
#include "RenderTexture.h"
#include "Texture.h"
#include "UniformBufferObject.h"

UniformBufferObject* RenderManager::CameraBuffer = nullptr;
RenderManager::CameraBufferData RenderManager::CameraData;

void RenderManager::Init()
{
    Object::Init();

    CameraBuffer = CreateObject<UniformBufferObject>(this);
}

void RenderManager::Shutdown()
{
    Object::Shutdown();
}

void RenderManager::BindMaterial(Material* Mat)
{
    if(BoundMat == Mat)
        return;

    if(BoundMat)
        BoundMat->MaterialUnbound();

    if(Mat->GetProgramID() != BoundProgramID)
        glUseProgram(Mat != nullptr ? Mat->GetProgramID() : 0);

    BoundProgramID = Mat->GetProgramID();
    BoundMat = Mat;

    if(BoundMat)
        BoundMat->MaterialBound();
}

void RenderManager::BindTexture(Texture* Tex, unsigned Index)
{
    if(BoundTextures[Index] == Tex)
        return;

    if(BoundTextures[Index])
        BoundTextures[Index]->TextureUnbound(Index);
    
    glActiveTexture(GL_TEXTURE0 + Index);
    glBindTexture(GL_TEXTURE_2D, Tex != nullptr ? Tex->GetTextureID() : 0);
    BoundTextures[Index] = Tex;

    if(BoundTextures[Index])
        BoundTextures[Index]->TextureBound(Index);
}

void RenderManager::BindFramebuffer(RenderTexture* FB)
{
    if(BoundFramebuffer == FB)
        return;

    if(BoundFramebuffer)
        BoundFramebuffer->FramebufferUnbound();
    
    glBindFramebuffer(GL_FRAMEBUFFER, FB != nullptr ? FB->GetFBO() : 0);
    BoundFramebuffer = FB;

    if(BoundFramebuffer)
        BoundFramebuffer->FramebufferBound();
}

void RenderManager::BindUniformBuffer(UniformBufferObject* Buf, unsigned Index)
{
    if(BoundBuffers[Index] == Buf)
        return;

    if(BoundBuffers[Index])
        BoundBuffers[Index]->BufferUnbound(Index);

    glBindBuffer(GL_UNIFORM_BUFFER, Buf->GetUBO());
    glBindBufferRange(GL_UNIFORM_BUFFER, Index, Buf != nullptr ? Buf->GetUBO() : 0, 0, Buf != nullptr ? Buf->GetSize() : 0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    BoundBuffers[Index] = Buf;

    if(BoundBuffers[Index])
        BoundBuffers[Index]->BufferBound(Index);
}

Material* RenderManager::GetMaterial() const
{
    return BoundMat;
}

Texture* RenderManager::GetTexture(unsigned Index) const
{
    return BoundTextures[Index];
}

RenderTexture* RenderManager::GetFramebuffer() const
{
    return BoundFramebuffer;
}

UniformBufferObject* RenderManager::GetUniformBuffer(unsigned Index) const 
{
    return BoundBuffers[Index];
}

void RenderManager::RenderScene(CameraComponent* Camera)
{
    CameraData.gEyePosition = Camera->GetPosition();
    CameraData.gProjectionView = Camera->GetProjectionView();
    CameraBuffer->BufferData(&CameraData);
    CameraBuffer->Bind(0);
    
    for(size_t i = 0; i < Drawables.size(); ++i)
    {
        Drawables[i]->Draw();
    }
}

void RenderManager::RegisterDrawable(IDrawable* Drawable)
{
    Drawables.push_back(Drawable);
}

void RenderManager::DeregisterDrawable(IDrawable* Drawable)
{
    std::remove(Drawables.begin(), Drawables.end(), Drawable);
}