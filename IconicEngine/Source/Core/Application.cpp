#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/detail/type_quat.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine.h"
#include "Rendering/Material.h"
#include "Rendering/RenderManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTexture2D.h"

#include "Core/Components/StaticMeshComponent.h"
#include "Core/Actors/FlyCamera.h"
#include "Core/Actors/StaticMeshActor.h"
#include "Core/Actors/PointLightActor.h"

void Application::Init()
{
    Actor::Init();
	
    RootActor = Engine::Get()->GetActiveWorld()->SpawnActor<Actor>();

    AssetManager* AM = Engine::Get()->GetAssetManager();
	PACKAGE_HANDLE SphereMeshHandle = AM->QueueImport("Content/Sphere.obj");
	SphereMesh = AM->GetFirstAssetOfTypeInPackage<StaticMesh>(SphereMeshHandle);
    
    PACKAGE_HANDLE Handle = AM->QueueImport("Content/sponza/sponza.obj");

    std::vector<StaticMesh*> StaticMeshes;
    AM->GetAllAssetsOfTypeInPackage<StaticMesh>(Handle, StaticMeshes);

    for(int i = 0; i < StaticMeshes.size(); ++i)
    {
        StaticMeshActor* SMActor = Engine::Get()->GetActiveWorld()->SpawnActor<StaticMeshActor>();
        SMActor->SetMesh(StaticMeshes[i]);
        SMActor->GetRootComponent()->SetLocalPosition(glm::vec3(0.0f, -150.0f, 0.0f));
        Meshes.push_back(SMActor);
    }
    
    FlyCam = Engine::Get()->GetActiveWorld()->SpawnActor<FlyCamera>();
    //FlyCam->SetRotation(glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    FlyCam->SetPosition(-FlyCam->GetForward() * 20.0f);
	GetRenderManager()->SetMainCamera(FlyCam->GetCameraComponent());

	for (unsigned int i = 0; i < NumLights; ++i)
	{
		float x = ((rand() % 10000) / 10000.0f) - 0.5f;
		float y = ((rand() % 10000) / 10000.0f) - 0.5f;
		float z = ((rand() % 10000) / 10000.0f) - 0.5f;
		float len = (rand() % 2000);

		glm::vec3 Position = glm::vec3(x, y, z);
		Position = glm::normalize(Position);
		Position *= len;

		Material* Mat = CreateObject<Material>(this);
		Mat->SetShader(Engine::BaseShader);

		float r = (rand() % 1000) / 1000.0f;
		float g = (rand() % 1000) / 1000.0f;
		float b = (rand() % 1000) / 1000.0f;

		Light = Engine::Get()->GetActiveWorld()->SpawnActor<PointLightActor>();
		Light->GetRootComponent()->AttachTo(RootActor->GetRootComponent());
		Light->GetPointLightComponent()->SetLightColor(glm::vec4(r, g, b, 1.0f));
		Light->SetPosition(glm::vec3(Position));

		//StaticMeshComponent* Comp = Light->AddComponent<StaticMeshComponent>();
		//Comp->AttachTo(Light->GetRootComponent());
		//Comp->SetMesh(SphereMesh);
		//Comp->SetMaterial(0, Mat);
		//Mat->SetVec3("gDiffuseColor", glm::vec3(Light->GetPointLightComponent()->GetLightColor()));
		//Mat->SetInt("gMaterialID", 0);
	}
}

void Application::Update(float DeltaTime)
{
    Actor::Update(DeltaTime);

	if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_F1))
	{
		GetRenderManager()->SetDrawOutputTarget(RenderManager::DrawOutputTarget::FinalColor);
	}
	if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_F2))
	{
		GetRenderManager()->SetDrawOutputTarget(RenderManager::DrawOutputTarget::Position);
	}
	if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_F3))
	{
		GetRenderManager()->SetDrawOutputTarget(RenderManager::DrawOutputTarget::Normal);
	}
	if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_F4))
	{
		GetRenderManager()->SetDrawOutputTarget(RenderManager::DrawOutputTarget::Ambient);
	}
	if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_F5))
	{
		GetRenderManager()->SetDrawOutputTarget(RenderManager::DrawOutputTarget::Albedo);
	}
	if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_F6))
	{
		GetRenderManager()->SetDrawOutputTarget(RenderManager::DrawOutputTarget::Spec);
	}
	if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_F7))
	{
		GetRenderManager()->SetDrawOutputTarget(RenderManager::DrawOutputTarget::Composited);
	}

	Angle += DeltaTime * 20.0f;
	glm::quat rot = glm::angleAxis(glm::radians(Angle), glm::vec3(0.0f, 1.0f, 0.0f));
	//RootActor->SetLocalRotation(rot);
}

void Application::Shutdown()
{
    Actor::Shutdown();
}
