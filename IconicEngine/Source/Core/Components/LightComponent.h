#pragma once
#include "Component.h"
#include "glm/vec4.hpp"
#include "glm/matrix.hpp"

class RenderTexture;
class Texture;

class LightComponent : public Component
{
public:
	IMPLEMENT_CONSTRUCTOR(LightComponent, Component);

	virtual void Init() override;
	virtual void Update(const float DeltaTime) override;
	virtual void Shutdown() override;
	glm::mat4 GetLightProjectionView() const;

	virtual void SetIntensity(float Intensity) = 0;
	virtual float GetIntensity() const = 0;

	virtual void SetColor(const glm::vec4& Color) = 0;
	virtual glm::vec4 GetColor() const = 0;

	virtual glm::mat4 GetLightProjection() const = 0;
	virtual glm::mat4 GetLightView() const = 0;

	virtual void GenerateShadowMap() = 0;
	virtual Texture* GetShadowMap() const = 0;
	virtual RenderTexture* GetShadowFramebuffer() const = 0;

private:
	glm::mat4 LightProjectionView;
};

