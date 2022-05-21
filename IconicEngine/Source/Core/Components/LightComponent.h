#pragma once
#include "Component.h"
#include "glm/vec4.hpp"

class LightComponent : public Component
{
public:
	struct Data
	{
		glm::vec4 Position;
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	};

	IMPLEMENT_CONSTRUCTOR(LightComponent, Component);

	virtual void Init() override;
	virtual void Update(const float DeltaTime) override;
	virtual void Shutdown() override;

	void SetSpecularColor(glm::vec3 NewSpecular);
	void SetDiffuseColor(glm::vec3 NewDiffuse);
	void SetAmbientColor(glm::vec3 NewAmbient);

	Data* GetLightData() const;

protected:
	virtual Data* MakeLightData();

private:
	Data* LightData;
};

