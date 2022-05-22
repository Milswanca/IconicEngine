#include "DirectionalLightComponent.h"
#include "Rendering/RenderTexture2D.h"

#include "glm/gtx/transform.hpp"

void DirectionalLightComponent::Init()
{
	LightComponent::Init();

	LightData = new Data();

	RenderTexture2D::CreateRenderTexture2DParams Params;
	Params.W = 1200;
	Params.H = 800;
	Params.DepthStencilRule = RenderTexture2D::AddDepthStencilRules::DepthOnly;
	ShadowMap = RenderTexture2D::Create(this, Params);
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

glm::mat4 DirectionalLightComponent::GetLightView() const
{
	return glm::lookAt(GetPosition(), GetPosition() + GetForward(), GetUp());
}

glm::mat4 DirectionalLightComponent::GetLightProjection() const
{
	float Near = 0.3f;
	float Far = 10000.0f;
	float FOV = 45.0f;
	float Aspect = 600.0f / 400.0f;
	return glm::perspective(FOV, Aspect, Near, Far);
}

void DirectionalLightComponent::GenerateShadowMap()
{
	ShadowMap->BindFramebuffer();
}

RenderTexture* DirectionalLightComponent::GetShadowMap() const
{
	return ShadowMap;
}

DirectionalLightComponent::Data* DirectionalLightComponent::GetDirectionalLightData() const
{
	return LightData;
}
