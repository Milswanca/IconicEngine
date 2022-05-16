#pragma once
#include "Texture.h"

class Texture;

class RenderTexture : public Texture
{
public:
    struct CreateRenderTextureParams
    {
        unsigned int W;
        unsigned int H;
        TextureFormats Format;
        unsigned char* Pixels;
        bool GenerateMips;
        bool AttachDepthBuffer;
    };

public:
    static RenderTexture* Create(Object* NewOuter, const CreateRenderTextureParams& Params);
    
    IMPLEMENT_CONSTRUCTOR(RenderTexture, Texture);

    virtual void Init() override;
    virtual void Shutdown() override;

    void Bind();
    void AttachDepthBuffer();
    virtual void UpdateResource() override;

    void Clear(bool CreateDepth);

    unsigned int GetFBO() const;
    unsigned int GetRBO() const;

protected:
    virtual void FramebufferBound();
    virtual void FramebufferUnbound();
    
private:
    GLuint RBO;
    GLuint FBO;

    friend class RenderManager;
};
