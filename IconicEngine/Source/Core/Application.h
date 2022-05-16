#pragma once
#include "Actor.h"
#include "FlyCamera.h"
#include "StaticMesh.h"
#include "StaticMeshActor.h"

class Material;
class Shader;
class RenderTexture;

class Application : public Actor
{    
public:
    IMPLEMENT_CONSTRUCTOR(Application, Actor);
    
    virtual void Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void Shutdown() override;

    Material* FullScreenQuadMat;
    RenderTexture* ScreenTexture;
    StaticMesh* QuadMesh;

    Shader* Program;
    Material* Mat;
    std::vector<StaticMeshActor*> Meshes;
    Actor* RootActor;
    float Angle = 0.0f;
    FlyCamera* FlyCam;
};
