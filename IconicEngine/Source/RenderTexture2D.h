#pragma once
#include "RenderTexture.h"

class Texture;

class RenderTexture2D : public RenderTexture
{
public:
	struct RenderTexture2DData
	{
		unsigned int Width;
		unsigned int Height;
		TextureFormats TextureFormat;
	};

	struct CreateRenderTexture2DParams
	{
		unsigned int W;
		unsigned int H;
		TextureFormats Format;
		unsigned char* Pixels;
		bool GenerateMips;
		bool AttachDepthBuffer;
	};

public:
	static RenderTexture2D* Create(Object* NewOuter, const CreateRenderTexture2DParams& Params);

	IMPLEMENT_CONSTRUCTOR(RenderTexture2D, RenderTexture);

	virtual void Init() override;
	virtual void Shutdown() override;

	Texture2D* AddTexture(unsigned int Index, TextureFormats Format, bool bUpdateResource = false);
	void AddDepthBuffer();
	Texture2D* GetTexture(unsigned int Index) const;
	virtual void UpdateResource() override;

	void Clear(bool CreateDepth);
	unsigned int GetRBO() const;

	virtual GLuint GetTextureID() const override;

protected:
	virtual void FramebufferBound() override;
	virtual void FramebufferUnbound() override;

private:
	GLuint RBO;
	RenderTexture2DData* Data;
	Texture2D* Textures[32];

	friend class RenderManager;
};
