#pragma once
#include "Core/Components/LightComponent.h"
#include "glm/vec4.hpp"
#include "glm/matrix.hpp"

class RenderTexture2D;

class DirectionalLightComponent : public LightComponent
{
public:
	struct Data
	{
		glm::vec4 Direction = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		float Intensity = 1.0f;
		float Padding[3];
	};

public:
	IMPLEMENT_CONSTRUCTOR(DirectionalLightComponent, LightComponent);

	virtual void Init() override;
	virtual void Update(const float DeltaTime) override;
	virtual void Shutdown() override;

	virtual void SetIntensity(float Intensity) override;
	virtual float GetIntensity() const override;

	virtual void SetColor(const glm::vec4& Color) override;
	virtual glm::vec4 GetColor() const override;

	virtual glm::mat4 GetLightView() const override;
	virtual glm::mat4 GetLightProjection() const override;

	virtual void GenerateShadowMap() override;
	virtual RenderTexture* GetShadowMap() const override;

	Data* GetDirectionalLightData() const;

protected:
	RenderTexture2D* ShadowMap;
	Data* LightData;
};

