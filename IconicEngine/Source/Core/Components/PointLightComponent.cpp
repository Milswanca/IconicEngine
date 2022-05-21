#include "PointLightComponent.h"

void PointLightComponent::SetLightColor(const glm::vec4& Color)
{
	static_cast<PointLightComponent::Data*>(GetLightData())->Color = Color;
}

glm::vec4 PointLightComponent::GetLightColor() const
{
	return static_cast<PointLightComponent::Data*>(GetLightData())->Color;
}

LightComponent::Data* PointLightComponent::MakeLightData()
{
	return new PointLightComponent::Data();
}
