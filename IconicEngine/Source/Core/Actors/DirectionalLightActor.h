#pragma once
#include "Core/Actor.h"

class DirectionalLightComponent;

class DirectionalLightActor : public Actor
{
public:
	IMPLEMENT_CONSTRUCTOR(DirectionalLightActor, Actor);

	virtual void Init() override;

	DirectionalLightComponent* GetDirectionalLightComponent() const;

private:
	DirectionalLightComponent* DirectionalLightComp;
};