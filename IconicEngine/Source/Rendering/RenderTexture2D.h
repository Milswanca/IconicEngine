#pragma once
#include "RenderTexture.h"
#include <unordered_map>

class Texture;

class RenderTexture2D : public RenderTexture
{
public:
	struct AddColorAttachmentParams
	{
		TextureFormats Format;

		AddColorAttachmentParams()
		{

		}

		AddColorAttachmentParams(TextureFormats InFormat)
		{
			Format = InFormat;
		}
	};

	enum class AddDepthStencilRules
	{
		None,
		DepthOnly,
		StencilOnly,
		DepthAndStencil
	};

	struct RenderTexture2DData
	{
		unsigned int Width;
		unsigned int Height;
	};

	struct CreateRenderTexture2DParams
	{
	public:
		unsigned int W = 0;
		unsigned int H = 0;
		unsigned int NumColorAttachments = 0;
		AddColorAttachmentParams ColorAttachments[32];
		AddDepthStencilRules DepthStencilRule;
	};

public:
	static RenderTexture2D* Create(Object* NewOuter, const CreateRenderTexture2DParams& Params);

	IMPLEMENT_CONSTRUCTOR(RenderTexture2D, RenderTexture);

	virtual void Init() override;
	virtual void Shutdown() override;

	Texture2D* AddColorAttachment(unsigned int Index, TextureFormats Format, bool bUpdateResource = false);
	Texture2D* AddDepthAttachment(bool bUpdateResource = false);
	Texture2D* AddStencilAttachment(bool bUpdateResource = false);
	Texture2D* AddDepthAndStencilAttachment(bool bUpdateResource = false);

	Texture2D* GetColorAttachment(unsigned int Index) const;
	Texture2D* GetDepthAttachment() const;
	Texture2D* GetStencilAttachment() const;

	virtual void UpdateResource() override;

	void Clear(bool CreateDepth);
	virtual GLuint GetTextureID() const override;

protected:
	virtual void FramebufferBound() override;
	virtual void FramebufferUnbound() override;

private:
	RenderTexture2DData* Data;
	Texture2D* ColorAttachments[32];
	Texture2D* DepthAttachment;
	Texture2D* StencilAttachment;

	friend class RenderManager;
};
