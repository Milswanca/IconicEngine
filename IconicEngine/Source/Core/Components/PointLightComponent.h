#pragma once
#include "Core/Components/LightComponent.h"

class RenderTexture2D;

class PointLightComponent : public LightComponent
{
public:
	struct Data
	{
		glm::vec4 Position;
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		float Intensity = 1.0f;
		float Radius = 300.0f;
		float Padding2[2];
	};

	IMPLEMENT_CONSTRUCTOR(PointLightComponent, LightComponent);

	virtual void Init() override;
	virtual void Update(const float DeltaTime) override;
	virtual void Shutdown() override;

	virtual void SetIntensity(float Intensity) override;
	virtual float GetIntensity() const;

	virtual void SetColor(const glm::vec4& Color) override;
	virtual glm::vec4 GetColor() const;

	virtual glm::mat4 GetLightProjection() const;
	virtual glm::mat4 GetLightView() const;

	virtual void GenerateShadowMap();
	virtual RenderTexture* GetShadowMap() const;

	void SetRadius(float Radius);
	float GetRadius() const;

	Data* GetPointLightData() const;

private:
	RenderTexture2D* ShadowMap;
	Data* LightData;
};

