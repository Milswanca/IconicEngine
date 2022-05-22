#include "PointLightComponent.h"
#include "glm/gtx/transform.hpp"
#include "Rendering/RenderTexture2D.h"

void PointLightComponent::Init()
{
	LightComponent::Init();

	LightData = new Data();

	RenderTexture2D::CreateRenderTexture2DParams Params;
	Params.W = 1200;
	Params.H = 800;
	Params.DepthStencilRule = RenderTexture2D::AddDepthStencilRules::DepthOnly;
	ShadowMap = RenderTexture2D::Create(this, Params);
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

glm::mat4 PointLightComponent::GetLightProjection() const
{
	return glm::identity<glm::mat4>();
}

glm::mat4 PointLightComponent::GetLightView() const
{
	return glm::identity<glm::mat4>();
}

void PointLightComponent::GenerateShadowMap()
{

}

RenderTexture* PointLightComponent::GetShadowMap() const
{
	return ShadowMap;
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
