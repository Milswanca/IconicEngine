#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/detail/type_quat.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine.h"
#include "Material.h"
#include "Shader.h"
#include "StaticMeshComponent.h"

Application::Application(Object* NewOuter) : Actor(NewOuter)
{
    RootActor = nullptr;    
}

Application::~Application()
{
}

void Application::Start()
{
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
}

void Application::Update(float DeltaTime)
{
    if(glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); 
    if(glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); 
    
    for(int i = 0; i < Meshes.size(); ++i)
    {
        Meshes[i]->Draw(FlyCam->GetPosition(), FlyCam->GetView(), FlyCam->GetProjection());
    }
}

void Application::Shutdown()
{
    
}
