#pragma once
#include "Actor.h"
#include "FlyCamera.h"
#include "StaticMesh.h"
#include "StaticMeshActor.h"

class Material;
class Shader;
class RenderTexture;
class RenderTexture2D;
class StaticMeshActor;

class Application : public Actor
{    
public:
    IMPLEMENT_CONSTRUCTOR(Application, Actor);
    
    virtual void Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void Shutdown() override;

    Material* GBufferMaterial;
    Material* FullScreenQuadMat;
    RenderTexture* ScreenTexture;
    StaticMesh* QuadMesh;
    RenderTexture2D* GBuffer;

    Shader* Program;
    Material* Mat;
    std::vector<StaticMeshActor*> Meshes;
    Actor* RootActor;
    float Angle = 0.0f;
    FlyCamera* FlyCam;
};
