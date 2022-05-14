#pragma once
#include <glm/ext/matrix_float4x4.hpp>

#include "Actor.h"
#include "FlyCamera.h"
#include "StaticMesh.h"
#include "StaticMeshActor.h"

class Material;
class Shader;

class Application : public Actor
{
public:
    Application(Object* NewOuter);
    virtual ~Application();
    
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void Shutdown();

    Shader* Program;
    Material* Mat;
    std::vector<StaticMeshActor*> Meshes;
    Actor* RootActor;
    float Angle = 0.0f;
    FlyCamera* FlyCam;
};
