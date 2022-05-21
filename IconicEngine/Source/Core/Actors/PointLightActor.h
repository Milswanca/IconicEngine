#pragma once
#include "Core/Actor.h"

class PointLightComponent;

class PointLightActor : public Actor
{
public:
	IMPLEMENT_CONSTRUCTOR(PointLightActor, Actor);

	virtual void Init() override;

	PointLightComponent* GetPointLightComponent() const;

private:
	PointLightComponent* PointLightComp;
};

