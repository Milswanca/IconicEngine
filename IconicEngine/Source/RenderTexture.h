#pragma once
#include "Texture.h"

class Texture;

class RenderTexture : public Texture
{
public:
    void Bind();
    void AttachDepthBuffer();
    virtual void UpdateResource(bool bGenerateMips) override;
    
private:
    unsigned int FBO;
};
