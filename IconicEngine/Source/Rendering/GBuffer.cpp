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

	for (unsigned int i = 0; i < MAX_GBUFFER_PASSES; ++i)
	{
		RemovePass(i);
	}
}

void GBuffer::AddPass(unsigned PassIndex, const GBufferPass& Pass)
{
	if (RenderPasses[PassIndex] != nullptr)
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
	GBufferSceneTargetParams.W = 1400;
	GBufferSceneTargetParams.H = 800;
	GBufferSceneTargetParams.NumColorAttachments = 5;
	GBufferSceneTargetParams.ColorAttachments[0] = RenderTexture2D::AddColorAttachmentParams(Texture::TextureFormats::RGB8);
	GBufferSceneTargetParams.ColorAttachments[1] = RenderTexture2D::AddColorAttachmentParams(Texture::TextureFormats::RGB8);
	GBufferSceneTargetParams.ColorAttachments[2] = RenderTexture2D::AddColorAttachmentParams(Texture::TextureFormats::RGB32);
	GBufferSceneTargetParams.ColorAttachments[3] = RenderTexture2D::AddColorAttachmentParams(Texture::TextureFormats::RGB32);
	GBufferSceneTargetParams.ColorAttachments[4] = RenderTexture2D::AddColorAttachmentParams(Texture::TextureFormats::RGB32);
	GBufferSceneTargetParams.DepthStencilRule = RenderTexture2D::AddDepthStencilRules::DepthOnly;
	GBufferSceneTarget = RenderTexture2D::Create(this, GBufferSceneTargetParams);

	RenderTexture2D::CreateRenderTexture2DParams GBufferCompositeTargetParams;
	GBufferCompositeTargetParams.W = 1400;
	GBufferCompositeTargetParams.H = 800;
	GBufferCompositeTargetParams.NumColorAttachments = 1;
	GBufferCompositeTargetParams.ColorAttachments[0] = RenderTexture2D::AddColorAttachmentParams(Texture::TextureFormats::RGB32);
	GBufferCompositeTarget = RenderTexture2D::Create(this, GBufferCompositeTargetParams);

	RenderTexture2D::CreateRenderTexture2DParams GBufferPostProcessTargetParams;
	GBufferPostProcessTargetParams.W = 1400;
	GBufferPostProcessTargetParams.H = 800;
	GBufferPostProcessTargetParams.NumColorAttachments = 1;
	GBufferPostProcessTargetParams.ColorAttachments[0] = RenderTexture2D::AddColorAttachmentParams(Texture::TextureFormats::RGB32);
	GBufferPostProcessTarget = RenderTexture2D::Create(this, GBufferPostProcessTargetParams);

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
	GBufferCompositeMaterial->SetTexture("gTex_Ambient", GBufferSceneTarget->GetColorAttachment(0));
	GBufferCompositeMaterial->SetTexture("gTex_Albedo", GBufferSceneTarget->GetColorAttachment(1));
	GBufferCompositeMaterial->SetTexture("gTex_Position", GBufferSceneTarget->GetColorAttachment(2));
	GBufferCompositeMaterial->SetTexture("gTex_Normal", GBufferSceneTarget->GetColorAttachment(3));
	GBufferCompositeMaterial->SetTexture("gTex_ShineMatID", GBufferSceneTarget->GetColorAttachment(4));

	PostProcessMaterial = CreateObject<Material>(this);
	PostProcessMaterial->SetShader(PostProcessShader);

	PostProcessMaterial->SetTexture("gTex_Scene", GBufferCompositeTarget->GetColorAttachment(0));

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

	switch (Pass)
	{
	case 0:
		glEnable(GL_DEPTH);
		GBufferSceneTarget->BindFramebuffer();
		GBufferSceneTarget->Clear(true);
		break;

	case 1:
		glEnable(GL_DEPTH);
		GBufferCompositeTarget->BindFramebuffer();
		GBufferCompositeTarget->Clear(true);
		GBufferCompositeMaterial->SetTexture("gTex_ShadowMap", GetRenderManager()->GetShadowCasterTEMP()->GetShadowMap());

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

		glm::mat4 ProjView = biasMatrix * GetRenderManager()->GetShadowCasterTEMP()->GetLightProjectionView();
		GBufferCompositeMaterial->SetMat4("gLightProjectionView", ProjView);
		GBufferCompositeMaterial->SetFloat("gLightNear", 0.03f);
		GBufferCompositeMaterial->SetFloat("gLightFar", 20.0f);
		break;

	case 2:
		GBufferPostProcessTarget->BindFramebuffer();
		GBufferPostProcessTarget->Clear(true);
	}
}

void GBufferDeferred::PostRenderPass(unsigned Pass)
{
	GBuffer::PostRenderPass(Pass);

	GetRenderManager()->BindFramebuffer(nullptr);
}

Texture2D* GBufferDeferred::GetPositionTexture() const
{
	return GBufferSceneTarget->GetColorAttachment(2);
}

Texture2D* GBufferDeferred::GetNormalTexture() const
{
	return GBufferSceneTarget->GetColorAttachment(3);
}

Texture2D* GBufferDeferred::GetAlbedoTexture() const
{
	return GBufferSceneTarget->GetColorAttachment(1);
}

Texture2D* GBufferDeferred::GetSpecularTexture() const
{
	return GBufferSceneTarget->GetColorAttachment(4);
}

Texture2D* GBufferDeferred::GetAmbientTexture() const
{
	return GBufferSceneTarget->GetColorAttachment(0);
}

Texture2D* GBufferDeferred::GetCompositedTexture() const
{
	return GBufferCompositeTarget->GetColorAttachment(0);
}

Texture2D* GBufferDeferred::GetFinalTexture() const
{
	return GBufferPostProcessTarget->GetColorAttachment(0);
}

void GBufferForward::Init()
{
}

void GBufferForward::Shutdown()
{

}

void GBufferForward::PreRenderPass(unsigned Pass)
{

}

void GBufferForward::PostRenderPass(unsigned Pass)
{

}
