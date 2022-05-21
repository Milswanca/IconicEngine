#include "LightComponent.h"
#include "Rendering/RenderManager.h"

void LightComponent::Init()
{
	Component::Init();

	LightData = MakeLightData();
	GetRenderManager()->RegisterLight(this);
}

void LightComponent::Update(const float DeltaTime)
{
	Component::Update(DeltaTime);

	LightData->Position = glm::vec4(GetPosition(), 1.0f);
}

void LightComponent::Shutdown()
{
	Component::Shutdown();

	GetRenderManager()->DeregisterLight(this);
	delete LightData;
}

void LightComponent::SetSpecularColor(glm::vec3 NewSpecular)
{
	//LightData->Specular = NewSpecular;
}

void LightComponent::SetDiffuseColor(glm::vec3 NewDiffuse)
{
	//LightData->Diffuse = NewDiffuse;
}

void LightComponent::SetAmbientColor(glm::vec3 NewAmbient)
{
	//LightData->Ambient = NewAmbient;
}

LightComponent::Data* LightComponent::GetLightData() const
{
	return LightData;
}

LightComponent::Data* LightComponent::MakeLightData()
{
	return new Data();
}