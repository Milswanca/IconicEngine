#include "RenderTexture.h"

void RenderTexture::Bind()
{
    
}

void RenderTexture::AttachDepthBuffer()
{
    
}

void RenderTexture::UpdateResource(bool bGenerateMips)
{
    Texture::UpdateResource(bGenerateMips);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GetTextureID(), 0); 
}
