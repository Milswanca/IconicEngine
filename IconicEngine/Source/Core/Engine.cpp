#include "Engine.h"
#include "Application.h"
#include "Assets/AssetManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"

Engine* Engine::Inst = nullptr;
Shader* Engine::BaseShader = nullptr;

Engine::Engine()
{
    Inst = this;
    ObjectIDs = new IndexArray<unsigned short>(MAX_OBJECTS);
    ObjectTable = new LookUpTable<Object, unsigned short>(MAX_OBJECTS);

    _Window = nullptr;
    _Application = nullptr;
    _World = CreateObject<World>(nullptr);
    _AssetManager = CreateObject<AssetManager>(nullptr);
}

Engine::~Engine()
{
    delete ObjectTable;
    delete ObjectIDs;
}

Engine* Engine::Get()
{
    return Inst;
}

Application* Engine::GetApplication() const
{
    return _Application;
}

World* Engine::GetActiveWorld() const
{
    return _World;
}

int Engine::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    _Window = glfwCreateWindow(1200, 800, "LearnOpenGL", NULL, NULL);
    if (_Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 1200, 800);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glfwSwapInterval(0);
    
    GLint MaxPatchVertices = 0;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
    printf("Max supported patch vertices %d\n", MaxPatchVertices);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    
    BaseShader = CreateObject<Shader>(nullptr);
    BaseShader->SetShaderSource(ShaderTypes::Vertex, "Content/VertexShader.shader");
    BaseShader->SetShaderSource(ShaderTypes::Fragment, "Content/FragmentShader.shader");
    //BaseShader->SetShaderSource(ShaderTypes::TessControl, "Content/TessControl.shader");
    //BaseShader->SetShaderSource(ShaderTypes::TessEval, "Content/TessEval.shader");

    _Application = _World->SpawnActor<Application>();
    _Application->Start();
    
    return 0;
}

void Engine::Run()
{
    while (!glfwWindowShouldClose(_Window))
    {
        glfwSwapBuffers(_Window);
        glfwPollEvents();

        glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Update();
    }
}

void Engine::Update()
{
    clock_t ThisClock = clock();
    float DeltaTime = (float)((ThisClock - LastClock) / 1000.0f);
    LastClock = ThisClock;

    std::string WindowTitle = std::to_string(1.0f / DeltaTime);
    glfwSetWindowTitle(GetWindow(), ("FPS" + WindowTitle).c_str());
    _Application->Update(DeltaTime);

    if(_World)
    {
        _World->Update(DeltaTime);
    }
}

int Engine::Shutdown()
{
    _Application->Shutdown();
    delete _Application;
    
    glfwTerminate();
    return 0;
}

GLFWwindow* Engine::GetWindow() const
{
    return _Window;
}

AssetManager* Engine::GetAssetManager() const
{
    return _AssetManager;
}
