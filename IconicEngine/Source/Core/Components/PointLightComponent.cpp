#include "PointLightComponent.h"

void PointLightComponent::SetConstant(float NewConstant)
{
	//static_cast<PointLightComponent::Data*>(GetLightData())->Constant = NewConstant;
}

void PointLightComponent::SetLinear(float NewLinear)
{
	//static_cast<PointLightComponent::Data*>(GetLightData())->Linear = NewLinear;
}

void PointLightComponent::SetQuadratic(float NewQuadratic)
{
	//static_cast<PointLightComponent::Data*>(GetLightData())->Quadratic = NewQuadratic;
}

LightComponent::Data* PointLightComponent::MakeLightData()
{
	return new PointLightComponent::Data();
}
