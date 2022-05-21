#include "LightComponent.h"
#include "Rendering/RenderManager.h"

void LightComponent::Init()
{
	Component::Init();

	GetRenderManager()->RegisterLight(this);
}

void LightComponent::Shutdown()
{
	Component::Shutdown();

	GetRenderManager()->DeregisterLight(this);
}