#include "LightComponent.h"
#include "Rendering/RenderManager.h"

void LightComponent::Init()
{
	Component::Init();

	GetRenderManager()->RegisterLight(this);

	//LightProjectionView = GetLightProjection() * GetLightView();
}

void LightComponent::Update(const float DeltaTime)
{
	Component::Update(DeltaTime);

	LightProjectionView = GetLightProjection() * GetLightView();
}

void LightComponent::Shutdown()
{
	Component::Shutdown();

	GetRenderManager()->DeregisterLight(this);
}

glm::mat4 LightComponent::GetLightProjectionView() const
{
	return LightProjectionView;
}