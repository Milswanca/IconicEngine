#include "DirectionalLightComponent.h"
#include "Rendering/RenderTexture2D.h"
#include "Core/Actor.h"

#include "glm/gtx/transform.hpp"
#include <Core/Components/CameraComponent.h>

void DirectionalLightComponent::Init()
{
	LightComponent::Init();

	LightData = new Data();

	RenderTexture2D::CreateRenderTexture2DParams Params;
	Params.W = 2048 * 2;
	Params.H = 2048 * 2;
	Params.DepthStencilRule = RenderTexture2D::AddDepthStencilRules::DepthOnly;
	ShadowMap = RenderTexture2D::Create(this, Params);

	Camera = GetOwner()->AddComponent<CameraComponent>();
	Camera->AttachTo(this);
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
	return Camera->GetView();
	return glm::lookAt(GetForward() * 30.f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 DirectionalLightComponent::GetLightProjection() const
{
	return Camera->GetProjection();
	float Near = 0.03f;
	float Far = 10.0f;
	float FOV = 45.0f;
	float Aspect = 1200.0f / 800.0f;
	float Size = 10.0f;
	return glm::ortho(-Size, Size, -Size, Size, -1.0f, Far);
}

void DirectionalLightComponent::GenerateShadowMap()
{
	ShadowMap->BindFramebuffer();
}

Texture* DirectionalLightComponent::GetShadowMap() const
{
	return ShadowMap->GetDepthAttachment();
}

RenderTexture* DirectionalLightComponent::GetShadowFramebuffer() const
{
	return ShadowMap;
}

DirectionalLightComponent::Data* DirectionalLightComponent::GetDirectionalLightData() const
{
	return LightData;
}
