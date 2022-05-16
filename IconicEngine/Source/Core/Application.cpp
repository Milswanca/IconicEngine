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
#include "RenderManager.h"
#include "Shader.h"
#include "StaticMeshComponent.h"
#include "RenderTexture.h"

void Application::Init()
{
    Actor::Init();

    RenderTexture::CreateRenderTextureParams RTParams;
    RTParams.W = 1200;
    RTParams.H = 800;
    RTParams.Format = Texture::TextureFormats::RGB8;
    RTParams.Pixels = nullptr;
    RTParams.AttachDepthBuffer = true;
    RTParams.GenerateMips = false;
    ScreenTexture = RenderTexture::Create(this, RTParams);

    StaticMesh::FCreateStaticMeshParams SMParams;
    SMParams.Positions = {
        glm::vec3(-1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f)
    };
    SMParams.UVs = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
    };
    SMParams.Triangles = { 
        { 0, 1, 2, 2, 1, 3 }
    };
    QuadMesh = StaticMesh::Create(this, SMParams);

    FullScreenQuadMat = CreateObject<Material>(this);
    FullScreenQuadMat->SetShader(Engine::Get()->DrawFullScreenQuadShader);
    QuadMesh->SetMaterial(0, FullScreenQuadMat);
    FullScreenQuadMat->SetTexture("gTex_Scene", ScreenTexture);

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
    Actor::Update(DeltaTime);

    if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_1) == GLFW_PRESS)
    {
        FullScreenQuadMat->SetFloat("gInvert_Power", 0.0f);
        FullScreenQuadMat->SetFloat("gGreyscale_Power", 0.0f);
    }
    else if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_2) == GLFW_PRESS)
    {
        FullScreenQuadMat->SetFloat("gInvert_Power", 1.0f);
        FullScreenQuadMat->SetFloat("gGreyscale_Power", 0.0f);
    }
    else if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_3) == GLFW_PRESS)
    {
        FullScreenQuadMat->SetFloat("gInvert_Power", 0.0f);
        FullScreenQuadMat->SetFloat("gGreyscale_Power", 1.0f);
    }
    
    ScreenTexture->Bind();
    ScreenTexture->Clear(true);
    glEnable(GL_DEPTH_TEST);
    GetRenderManager()->RenderScene(FlyCam->GetCameraComponent());

    glDisable(GL_DEPTH_TEST);
    GetRenderManager()->BindFramebuffer(nullptr);

    QuadMesh->Draw(glm::identity<glm::mat4>());
}

void Application::Shutdown()
{
    Actor::Shutdown();
}
