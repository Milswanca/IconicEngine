#include "Texture2D.h"
#include "RenderManager.h"
#include "Core/Engine.h"

Texture2D* Texture2D::Create(Object* NewOuter, const CreateTexture2DParams& Params)
{
	Texture2D* Tex = Engine::Get()->CreateObject<Texture2D>(NewOuter);
	Tex->Data->Width = Params.W;
	Tex->Data->Height = Params.H;
	Tex->Data->TextureFormat = Params.Format;
	Tex->SetPixels(Params.Pixels);
	Tex->bGenerateMips = Params.GenerateMips;
	Tex->UpdateResource();

	return Tex;
}

void Texture2D::Init()
{
	Texture::Init();

	Data = new Texture2DData();
	glGenTextures(1, &TextureID);
}

void Texture2D::Shutdown()
{
	Texture::Shutdown();

	if (Data->PixelData != nullptr)
	{
		delete[] Data->PixelData;
		Data->PixelData = nullptr;
	}

	delete Data;
	glDeleteTextures(1, &TextureID);
}

void Texture2D::SetPixels(unsigned char* Pixels)
{
	if (Data->PixelData != nullptr)
	{
		delete[] Data->PixelData;
		Data->PixelData = nullptr;
	}

	if (Pixels == nullptr)
	{
		return;
	}

	unsigned int NumBytes = Data->Width * Data->Height * TexFormatsToGLTypes[Data->TextureFormat].Stride;
	Data->PixelData = new unsigned char[NumBytes];
	memcpy(Data->PixelData, Pixels, NumBytes);
}

void Texture2D::UpdateResource()
{
	Texture::UpdateResource();

	Texture* BoundTexture = GetRenderManager()->GetTexture(0);
	Bind(0);

	TexFormatToGLType GLData = TexFormatsToGLTypes[Data->TextureFormat];
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GLData.SourceFormat, Data->Width, Data->Height, 0, GLData.DesiredFormat, GLData.DataType, Data->PixelData);

	float aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

	if (bGenerateMips)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	GetRenderManager()->BindTexture(BoundTexture, 0);
}

GLuint Texture2D::GetTextureID() const
{
	return TextureID;
}