#include "RenderTexture2D.h"
#include <Core/Engine.h>
#include "RenderManager.h"

RenderTexture2D* RenderTexture2D::Create(Object* NewOuter, const CreateRenderTexture2DParams& Params)
{
	RenderTexture2D* Tex = Engine::Get()->CreateObject<RenderTexture2D>(NewOuter);
	Tex->Data->Width = Params.W;
	Tex->Data->Height = Params.H;

	for (unsigned int i = 0; i < Params.NumColorAttachments; ++i)
	{
		Tex->AddColorAttachment(i, Params.ColorAttachments[i].Format);
	}

	switch (Params.DepthStencilRule)
	{
	case AddDepthStencilRules::DepthOnly:
		Tex->AddDepthAttachment();
		break;

	case AddDepthStencilRules::StencilOnly:
		Tex->AddStencilAttachment();
		break;

	case AddDepthStencilRules::DepthAndStencil:
		Tex->AddDepthAndStencilAttachment();
		break;
	}

	Tex->UpdateResource();
	return Tex;
}

void RenderTexture2D::Init()
{
	RenderTexture::Init();

	Data = new RenderTexture2DData();
}

void RenderTexture2D::Shutdown()
{
	RenderTexture::Shutdown();
	delete Data;
}

Texture2D* RenderTexture2D::AddColorAttachment(unsigned int Index, TextureFormats Format, bool bUpdateResource)
{
	Texture2D::CreateTexture2DParams Params;
	Params.W = Data->Width;
	Params.H = Data->Height;
	Params.GenerateMips = false;
	Params.Format = Format;
	Params.Pixels = nullptr;

	if (ColorAttachments[Index] != nullptr)
	{
		GetColorAttachment(Index)->Destroy();
	}

	Texture2D* NewTex = Texture2D::Create(this, Params);
	ColorAttachments[Index] = NewTex;

	if (bUpdateResource)
	{
		UpdateResource();
	}

	return NewTex;
}

Texture2D* RenderTexture2D::AddDepthAttachment(bool bUpdateResource)
{
	Texture2D::CreateTexture2DParams Params;
	Params.W = Data->Width;
	Params.H = Data->Height;
	Params.GenerateMips = false;
	Params.Format = TextureFormats::Depth;
	Params.Pixels = nullptr;

	if (DepthAttachment != nullptr)
	{
		GetDepthAttachment()->Destroy();
	}

	Texture2D* NewTex = Texture2D::Create(this, Params);
	DepthAttachment = NewTex;

	if (bUpdateResource)
	{
		UpdateResource();
	}

	return NewTex;
}

Texture2D* RenderTexture2D::AddStencilAttachment(bool bUpdateResource)
{
	Texture2D::CreateTexture2DParams Params;
	Params.W = Data->Width;
	Params.H = Data->Height;
	Params.GenerateMips = false;
	Params.Format = TextureFormats::Depth;
	Params.Pixels = nullptr;

	if (StencilAttachment != nullptr)
	{
		GetStencilAttachment()->Destroy();
	}

	Texture2D* NewTex = Texture2D::Create(this, Params);
	StencilAttachment = NewTex;

	if (bUpdateResource)
	{
		UpdateResource();
	}

	return NewTex;
}

Texture2D* RenderTexture2D::AddDepthAndStencilAttachment(bool bUpdateResource)
{
	Texture2D::CreateTexture2DParams Params;
	Params.W = Data->Width;
	Params.H = Data->Height;
	Params.GenerateMips = false;
	Params.Format = TextureFormats::Depth;
	Params.Pixels = nullptr;

	if (StencilAttachment != nullptr)
	{
		GetStencilAttachment()->Destroy();
	}
	if (DepthAttachment != nullptr)
	{
		GetDepthAttachment()->Destroy();
	}

	Texture2D* NewTex = Texture2D::Create(this, Params);
	StencilAttachment = NewTex;
	DepthAttachment = NewTex;

	if (bUpdateResource)
	{
		UpdateResource();
	}

	return NewTex;
}

Texture2D* RenderTexture2D::GetColorAttachment(unsigned int Index) const
{
	return ColorAttachments[Index];
}

Texture2D* RenderTexture2D::GetDepthAttachment() const
{
	return DepthAttachment;
}

Texture2D* RenderTexture2D::GetStencilAttachment() const
{
	return StencilAttachment;
}

void RenderTexture2D::UpdateResource()
{
	RenderTexture::UpdateResource();

	RenderTexture* LastBound = GetRenderManager()->GetFramebuffer();

	BindFramebuffer();

	GLenum BoundBuffers[32];
	unsigned int NumBoundBuffers = 0;
	for (unsigned int i = 0; i < 32; ++i)
	{
		if (!ColorAttachments[i])
			continue;

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ColorAttachments[i]->GetTextureID(), 0);
		BoundBuffers[NumBoundBuffers] = GL_COLOR_ATTACHMENT0 + i;
		NumBoundBuffers++;
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment != nullptr ? DepthAttachment->GetTextureID() : 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, StencilAttachment != nullptr ? StencilAttachment->GetTextureID() : 0, 0);
	glDrawBuffers(NumBoundBuffers, BoundBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	GetRenderManager()->BindFramebuffer(LastBound);
}

void RenderTexture2D::Clear(bool ClearDepth)
{
	RenderTexture* LastBound = GetRenderManager()->GetFramebuffer();

	BindFramebuffer();

	GLenum Buffers = ClearDepth ? GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT : GL_COLOR_BUFFER_BIT;
	glClear(Buffers);

	GetRenderManager()->BindFramebuffer(LastBound);
}

GLuint RenderTexture2D::GetTextureID() const
{
	return ColorAttachments[0] != nullptr ? ColorAttachments[0]->GetTextureID() : 0;
}

void RenderTexture2D::FramebufferBound()
{

}

void RenderTexture2D::FramebufferUnbound()
{

}