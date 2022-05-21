#include "PointLightComponent.h"

void PointLightComponent::Init()
{
	LightComponent::Init();

	LightData = new Data();
}

void PointLightComponent::Update(const float DeltaTime)
{
	LightComponent::Update(DeltaTime);

	LightData->Position = glm::vec4(GetPosition(), 1.0f);
}

void PointLightComponent::Shutdown()
{
	LightComponent::Shutdown();

	delete LightData;
}

void PointLightComponent::SetIntensity(float Intensity)
{
	LightData->Intensity = Intensity;
}

float PointLightComponent::GetIntensity() const
{
	return LightData->Intensity;
}

void PointLightComponent::SetColor(const glm::vec4& Color)
{
	LightData->Color = Color;
}

glm::vec4 PointLightComponent::GetColor() const
{
	return LightData->Color;
}

void PointLightComponent::SetRadius(float Radius)
{
	LightData->Radius = Radius;
}

float PointLightComponent::GetRadius() const
{
	return LightData->Radius;
}

PointLightComponent::Data* PointLightComponent::GetPointLightData() const
{
	return LightData;
}
