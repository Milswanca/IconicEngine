#pragma once
#include "Texture.h"

class Texture2D : public Texture
{
public:
	struct Texture2DData
	{
		unsigned int Width;
		unsigned int Height;

		TextureFormats TextureFormat;
		unsigned char* PixelData;
	};

	struct CreateTexture2DParams
	{
		unsigned int W;
		unsigned int H;
		unsigned char* Pixels;
		TextureFormats Format;
		bool GenerateMips;
	};

public:
	static Texture2D* Create(Object* NewOuter, const CreateTexture2DParams& Params);

	IMPLEMENT_CONSTRUCTOR(Texture2D, Texture);

	virtual void Init() override;
	virtual void Shutdown() override;

	void SetPixels(unsigned char* Pixels);
	virtual void UpdateResource() override;

	virtual GLuint GetTextureID() const override;

protected:
	Texture2DData* Data;
	GLuint TextureID;

private:
	friend class RenderManager;
};

