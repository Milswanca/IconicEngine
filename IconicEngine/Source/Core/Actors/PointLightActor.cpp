#include "PointLightActor.h"
#include "Core/Components/PointLightComponent.h"

void PointLightActor::Init()
{
	Actor::Init();

	PointLightComp = AddComponent<PointLightComponent>();
	PointLightComp->AttachTo(GetRootComponent());
}

PointLightComponent* PointLightActor::GetPointLightComponent() const
{
	return PointLightComp;
}
