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
#include "RenderTexture2D.h"

void Application::Init()
{
    Actor::Init();

    Shader* GBufferShader = CreateObject<Shader>(this);
	GBufferShader->SetShaderSource(ShaderTypes::Vertex, "Content\\Shaders\\GBufferPassVS.shader");
	GBufferShader->SetShaderSource(ShaderTypes::Fragment, "Content\\Shaders\\GBufferPassFS.shader");
    GBufferShader->Compile();

    GBufferMaterial = CreateObject<Material>(this);
    GBufferMaterial->SetShader(GBufferShader);

	RenderTexture2D::CreateRenderTexture2DParams RTParams;
	RTParams.W = 1200;
	RTParams.H = 800;
	RTParams.Format = Texture::TextureFormats::RGB8;
	RTParams.Pixels = nullptr;
	RTParams.AttachDepthBuffer = true;
	RTParams.GenerateMips = false;
	ScreenTexture = RenderTexture2D::Create(this, RTParams);

	RenderTexture2D::CreateRenderTexture2DParams GBufferParams;
	GBufferParams.W = 1200;
	GBufferParams.H = 800;
	GBufferParams.Format = Texture::TextureFormats::RGB8;
	GBufferParams.Pixels = nullptr;
	GBufferParams.AttachDepthBuffer = true;
	GBufferParams.GenerateMips = false;
	GBuffer = RenderTexture2D::Create(this, GBufferParams);

	GBuffer->AddTexture(0, Texture::TextureFormats::RGBA8); // Ambient
	GBuffer->AddTexture(1, Texture::TextureFormats::RGBA8); // Albedo Spec
	GBuffer->AddTexture(2, Texture::TextureFormats::RGB32); // Position
	GBuffer->AddTexture(3, Texture::TextureFormats::RGB32); // Normal
    GBuffer->UpdateResource();

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
	else if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_4) == GLFW_PRESS)
	{
		FullScreenQuadMat->SetTexture("gTex_Scene", ScreenTexture);
	}
	else if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_5) == GLFW_PRESS)
	{
        FullScreenQuadMat->SetTexture("gTex_Scene", GBuffer->GetTexture(0));
	}
	else if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_6) == GLFW_PRESS)
	{
        FullScreenQuadMat->SetTexture("gTex_Scene", GBuffer->GetTexture(1));
	}
	else if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_7) == GLFW_PRESS)
	{
        FullScreenQuadMat->SetTexture("gTex_Scene", GBuffer->GetTexture(2));
	}
	else if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_8) == GLFW_PRESS)
	{
        FullScreenQuadMat->SetTexture("gTex_Scene", GBuffer->GetTexture(3));
	}

	ScreenTexture->BindFramebuffer();
    ScreenTexture->Clear(true);
	glEnable(GL_DEPTH_TEST);
	GetRenderManager()->RenderScene(FlyCam->GetCameraComponent());
    
    GBuffer->BindFramebuffer();
    GBuffer->Clear(true);
    glEnable(GL_DEPTH_TEST);
    GetRenderManager()->RenderScene(GBufferMaterial, FlyCam->GetCameraComponent());

    glDisable(GL_DEPTH_TEST);
    GetRenderManager()->BindFramebuffer(nullptr);
    GetRenderManager()->RenderMesh(FlyCam->GetCameraComponent(), glm::identity<glm::mat4>(), FullScreenQuadMat, QuadMesh);
}

void Application::Shutdown()
{
    Actor::Shutdown();
}
