#pragma once
#include "Component.h"
#include "glm/vec4.hpp"

class LightComponent : public Component
{
public:
	IMPLEMENT_CONSTRUCTOR(LightComponent, Component);

	virtual void Init() override;
	virtual void Shutdown() override;

	virtual void SetIntensity(float Intensity) = 0;
	virtual float GetIntensity() const = 0;

	virtual void SetColor(const glm::vec4& Color) = 0;
	virtual glm::vec4 GetColor() const = 0;
};

