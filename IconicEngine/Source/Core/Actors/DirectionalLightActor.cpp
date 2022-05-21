#include "DirectionalLightActor.h"
#include "Core/Components/DirectionalLightComponent.h"

void DirectionalLightActor::Init()
{
	Actor::Init();

	DirectionalLightComp = AddComponent<DirectionalLightComponent>();
	DirectionalLightComp->AttachTo(GetRootComponent());
}

DirectionalLightComponent* DirectionalLightActor::GetDirectionalLightComponent() const
{
	return DirectionalLightComp;
}
