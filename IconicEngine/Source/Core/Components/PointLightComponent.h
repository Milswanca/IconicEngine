#pragma once
#include "Core/Components/LightComponent.h"

class PointLightComponent : public LightComponent
{
public:
	struct Data : public LightComponent::Data
	{
		float Radius = 1000.0f;
		glm::vec3 Padding;
	};

	IMPLEMENT_CONSTRUCTOR(PointLightComponent, LightComponent);

	void SetConstant(float NewConstant);
	void SetLinear(float NewLinear);
	void SetQuadratic(float NewQuadratic);

protected:
	virtual LightComponent::Data* MakeLightData() override;
};

