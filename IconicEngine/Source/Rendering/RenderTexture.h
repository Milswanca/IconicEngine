#pragma once
#include "Texture2D.h"

class Texture;

class RenderTexture : public Texture
{
public:
	enum class RenderTextureBindings
	{
        ColorAttachment,
        DepthAttachment,
        StencilAttachment,
        DepthAndStencilAttachment,
	};

public:
    IMPLEMENT_CONSTRUCTOR(RenderTexture, Texture);

    virtual void Init() override;
    virtual void Shutdown() override;

    void BindFramebuffer();
    void Clear(bool ClearDepth);

    unsigned int GetFBO() const;

protected:
    virtual void FramebufferBound();
    virtual void FramebufferUnbound();
    
private:
    GLuint FBO;

    friend class RenderManager;
};
