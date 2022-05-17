#include "RenderTexture2D.h"
#include <Core/Engine.h>
#include "RenderManager.h"

RenderTexture2D* RenderTexture2D::Create(Object* NewOuter, const CreateRenderTexture2DParams& Params)
{
	RenderTexture2D* Tex = Engine::Get()->CreateObject<RenderTexture2D>(NewOuter);
	Tex->Data->Width = Params.W;
	Tex->Data->Height = Params.H;
	Tex->Data->TextureFormat = Params.Format;
	Tex->bGenerateMips = Params.GenerateMips;
	Tex->AddTexture(0, Params.Format);

	if (Params.AttachDepthBuffer)
	{
		Tex->AddDepthBuffer();
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

	if (RBO)
		glDeleteRenderbuffers(1, &RBO);
}

Texture2D* RenderTexture2D::AddTexture(unsigned int Index, TextureFormats Format, bool bUpdateResource)
{
	Texture2D::CreateTexture2DParams Params;
	Params.W = Data->Width;
	Params.H = Data->Height;
	Params.GenerateMips = false;
	Params.Format = Format;
	Params.Pixels = nullptr;

	if (Textures[Index] != nullptr)
	{
		Textures[Index]->Destroy();
	}

	Texture2D* NewTex = Texture2D::Create(this, Params);
	Textures[Index] = NewTex;

	if (bUpdateResource)
	{
		UpdateResource();
	}

	return NewTex;
}

void RenderTexture2D::AddDepthBuffer()
{
	if (RBO)
		return;

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Data->Width, Data->Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
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
		if (!Textures[i])
			continue;

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, Textures[i]->GetTextureID(), 0);
		BoundBuffers[NumBoundBuffers] = GL_COLOR_ATTACHMENT0 + i;
		NumBoundBuffers++;
	}

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	glDrawBuffers(NumBoundBuffers, BoundBuffers);

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

unsigned RenderTexture2D::GetRBO() const
{
	return RBO;
}

GLuint RenderTexture2D::GetTextureID() const
{
	return Textures[0]->GetTextureID();
}

void RenderTexture2D::FramebufferBound()
{
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
}

void RenderTexture2D::FramebufferUnbound()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Texture2D* RenderTexture2D::GetTexture(unsigned int Index) const
{
	return Textures[Index];
}