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

void Application::Init()
{
    Actor::Init();
	
    RootActor = Engine::Get()->GetActiveWorld()->SpawnActor<Actor>();
    
    AssetManager* AM = Engine::Get()->GetAssetManager();
    PACKAGE_HANDLE Handle = AM->QueueImport("Content/sponza/sponza.obj");

    std::vector<StaticMesh*> StaticMeshes;
    AM->GetAllAssetsOfTypeInPackage<StaticMesh>(Handle, StaticMeshes);

    for(int i = 0; i < StaticMeshes.size(); ++i)
    {
        StaticMeshActor* SMActor = Engine::Get()->GetActiveWorld()->SpawnActor<StaticMeshActor>();
        SMActor->SetMesh(StaticMeshes[i]);
        SMActor->GetRootComponent()->AttachTo(RootActor->GetRootComponent());
        SMActor->GetRootComponent()->SetLocalPosition(glm::vec3(0.0f, -150.0f, 0.0f));
        Meshes.push_back(SMActor);
    }
    
    FlyCam = Engine::Get()->GetActiveWorld()->SpawnActor<FlyCamera>();
    //FlyCam->SetRotation(glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    FlyCam->SetPosition(-FlyCam->GetForward() * 20.0f);
	GetRenderManager()->SetMainCamera(FlyCam->GetCameraComponent());
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
}

void Application::Shutdown()
{
    Actor::Shutdown();
}
