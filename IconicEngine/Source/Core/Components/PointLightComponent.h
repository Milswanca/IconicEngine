#pragma once
#include "Core/Components/LightComponent.h"

class PointLightComponent : public LightComponent
{
public:
	struct Data : public LightComponent::Data
	{
		float Radius = 300.0f;
		glm::vec3 Padding;
	};

	IMPLEMENT_CONSTRUCTOR(PointLightComponent, LightComponent);

	void SetLightColor(const glm::vec4& Color);
	glm::vec4 GetLightColor() const;

protected:
	virtual LightComponent::Data* MakeLightData() override;
};

