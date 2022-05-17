#include "GBuffer.h"

#include <cstring>

#include "Material.h"
#include "RenderManager.h"
#include "RenderTexture2D.h"
#include "Shader.h"

void GBuffer::Init()
{
    Object::Init();

    for (unsigned int i = 0; i < MAX_GBUFFER_PASSES; ++i)
    {
        RenderPasses[i] = nullptr;
    }
}

void GBuffer::Shutdown()
{
    Object::Shutdown();

    for(unsigned int i = 0; i < MAX_GBUFFER_PASSES; ++i)
    {
        RemovePass(i);
    }
}

void GBuffer::AddPass(unsigned PassIndex, const GBufferPass& Pass)
{
    if(RenderPasses[PassIndex] != nullptr)
        RemovePass(PassIndex);

    RenderPasses[PassIndex] = new GBufferPass();
    memcpy(RenderPasses[PassIndex], &Pass, sizeof(GBufferPass));
}

void GBuffer::RemovePass(unsigned PassIndex)
{
    delete RenderPasses[PassIndex];
    RenderPasses[PassIndex] = nullptr;
}

GBuffer::GBufferPass* GBuffer::GetPassData(unsigned int PassIndex) const
{
    return RenderPasses[PassIndex];
}

void GBuffer::PreRenderPass(unsigned Pass)
{
}

void GBuffer::PostRenderPass(unsigned Pass)
{
}

void GBufferDeferred::Init()
{
    GBuffer::Init();

    RenderTexture2D::CreateRenderTexture2DParams GBufferSceneTargetParams;
    GBufferSceneTargetParams.Format = Texture::TextureFormats::RGB8;
    GBufferSceneTargetParams.W = 1200;
    GBufferSceneTargetParams.H = 800;
    GBufferSceneTargetParams.AttachDepthBuffer = true;
    GBufferSceneTarget = RenderTexture2D::Create(this, GBufferSceneTargetParams);
    GBufferSceneTarget->AddTexture(1, Texture::TextureFormats::RGB8);
    GBufferSceneTarget->AddTexture(2, Texture::TextureFormats::RGB32);
    GBufferSceneTarget->AddTexture(3, Texture::TextureFormats::RGB32);
    GBufferSceneTarget->AddTexture(4, Texture::TextureFormats::RGB32);
    GBufferSceneTarget->UpdateResource();

    RenderTexture2D::CreateRenderTexture2DParams GBufferCompositeTargetParams;
    GBufferCompositeTargetParams.Format = Texture::TextureFormats::RGBA32;
    GBufferCompositeTargetParams.W = 1200;
    GBufferCompositeTargetParams.H = 800;
    GBufferCompositeTargetParams.AttachDepthBuffer = false;
    GBufferCompositeTarget = RenderTexture2D::Create(this, GBufferCompositeTargetParams);
    GBufferCompositeTarget->UpdateResource();

    GBufferSceneShader = CreateObject<Shader>(this);
    GBufferSceneShader->SetShaderSource(ShaderTypes::Vertex, "Content\\Shaders\\GBufferSceneVS.shader");
    GBufferSceneShader->SetShaderSource(ShaderTypes::Fragment, "Content\\Shaders\\GBufferSceneFS.shader");
    GBufferSceneShader->Compile();

    GBufferCompositeShader = CreateObject<Shader>(this);
    GBufferCompositeShader->SetShaderSource(ShaderTypes::Vertex, "Content\\Shaders\\GBufferCompositeVS.shader");
    GBufferCompositeShader->SetShaderSource(ShaderTypes::Fragment, "Content\\Shaders\\GBufferCompositeFS.shader");
    GBufferCompositeShader->Compile();

    PostProcessShader = CreateObject<Shader>(this);
    PostProcessShader->SetShaderSource(ShaderTypes::Vertex, "Content\\Shaders\\PostProcessVS.shader");
    PostProcessShader->SetShaderSource(ShaderTypes::Fragment, "Content\\Shaders\\PostProcessFS.shader");
    PostProcessShader->Compile();

    GBufferCompositeMaterial = CreateObject<Material>(this);
    GBufferCompositeMaterial->SetShader(GBufferCompositeShader);
    GBufferCompositeMaterial->SetTexture("gTex_Ambient", GBufferSceneTarget->GetTexture(0));
    GBufferCompositeMaterial->SetTexture("gTex_Albedo", GBufferSceneTarget->GetTexture(1));
    GBufferCompositeMaterial->SetTexture("gTex_Position", GBufferSceneTarget->GetTexture(2));
    GBufferCompositeMaterial->SetTexture("gTex_Normal", GBufferSceneTarget->GetTexture(3));
    GBufferCompositeMaterial->SetTexture("gTex_Shine", GBufferSceneTarget->GetTexture(4));

    PostProcessMaterial = CreateObject<Material>(this);
    PostProcessMaterial->SetShader(PostProcessShader);

    PostProcessMaterial->SetTexture("gTex_Scene", GBufferCompositeTarget->GetTexture(0));

    GBufferPass Pass0;
    Pass0.RenderType = GBufferRenderPassType::RenderScene;
    Pass0.RenderShader = GBufferSceneShader;
    AddPass(0, Pass0);

    GBufferPass Pass1;
    Pass1.RenderType = GBufferRenderPassType::RenderQuad;
    Pass1.RenderMaterial = GBufferCompositeMaterial;
    AddPass(1, Pass1);

    GBufferPass Pass2;
    Pass2.RenderType = GBufferRenderPassType::RenderQuad;
    Pass2.RenderMaterial = PostProcessMaterial;
    AddPass(2, Pass2);
}

void GBufferDeferred::Shutdown()
{
    GBuffer::Shutdown();
}

void GBufferDeferred::PreRenderPass(unsigned Pass)
{
    GBuffer::PreRenderPass(Pass);

    switch(Pass)
    {
    case 0:
        GBufferSceneTarget->BindFramebuffer();
        GBufferSceneTarget->Clear(true);
        break;

    case 1:
        GBufferCompositeTarget->BindFramebuffer();
        GBufferCompositeTarget->Clear(true);
        break;
    }
}

void GBufferDeferred::PostRenderPass(unsigned Pass)
{
    GBuffer::PostRenderPass(Pass);

    switch(Pass)
    {
    case 0:
        GetRenderManager()->BindFramebuffer(nullptr);
        break;

    case 1:
        GetRenderManager()->BindFramebuffer(nullptr);
        break;
    }
}
