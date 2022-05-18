#pragma once
#include "Core/Object.h"

class Material;
class Shader;
class Texture2D;
class RenderTexture2D;

class GBuffer : public Object
{
public:
    enum GBufferRenderPassType
    {
        RenderScene,
        RenderQuad
    };
    
    static const unsigned int MAX_GBUFFER_PASSES = 8;

    struct GBufferPass
    {
        GBufferRenderPassType RenderType;
        Shader* RenderShader;
        Material* RenderMaterial;
    };
    
public:
    IMPLEMENT_CONSTRUCTOR(GBuffer, Object);

    virtual void Init() override;
    virtual void Shutdown() override;
    
    void AddPass(unsigned int PassIndex, const GBufferPass& Pass);
    void RemovePass(unsigned int PassIndex);
    GBufferPass* GetPassData(unsigned int PassIndex) const;

protected:
    virtual void PreRenderPass(unsigned int Pass);
    virtual void PostRenderPass(unsigned int Pass);
    
private:
    GBufferPass* RenderPasses[MAX_GBUFFER_PASSES];

    friend class RenderManager;
};

class GBufferDeferred : public GBuffer
{
public:
    IMPLEMENT_CONSTRUCTOR(GBufferDeferred, GBuffer);

    virtual void Init() override;
    virtual void Shutdown() override;

    Texture2D* GetPositionTexture() const;
    Texture2D* GetNormalTexture() const;
    Texture2D* GetAlbedoTexture() const;
    Texture2D* GetSpecularTexture() const;
	Texture2D* GetAmbientTexture() const;
	Texture2D* GetCompositedTexture() const;
	Texture2D* GetFinalTexture() const;
    
protected:    
    virtual void PreRenderPass(unsigned Pass) override;
    virtual void PostRenderPass(unsigned Pass) override;

private:
    Shader* GBufferSceneShader;
    Shader* GBufferCompositeShader;
    Shader* PostProcessShader;
    Material* GBufferCompositeMaterial;
    Material* PostProcessMaterial;

    RenderTexture2D* GBufferSceneTarget;
	RenderTexture2D* GBufferCompositeTarget;
	RenderTexture2D* GBufferPostProcessTarget;
};