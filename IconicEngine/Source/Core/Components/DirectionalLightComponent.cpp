#include "DirectionalLightComponent.h"

void DirectionalLightComponent::Init()
{
	LightComponent::Init();

	LightData = new Data();
}

void DirectionalLightComponent::Update(const float DeltaTime)
{
	LightComponent::Update(DeltaTime);

	glm::vec3 Forward = GetForward();
	LightData->Direction = glm::vec4(Forward, 1.0f);
}

void DirectionalLightComponent::Shutdown()
{
	LightComponent::Shutdown();

	delete LightData;
}

void DirectionalLightComponent::SetIntensity(float Intensity)
{
	LightData->Intensity = Intensity;
}

float DirectionalLightComponent::GetIntensity() const
{
	return LightData->Intensity;
}

void DirectionalLightComponent::SetColor(const glm::vec4& Color)
{
	LightData->Color = Color;
}

glm::vec4 DirectionalLightComponent::GetColor() const
{
	return LightData->Color;
}

DirectionalLightComponent::Data* DirectionalLightComponent::GetDirectionalLightData() const
{
	return LightData;
}
