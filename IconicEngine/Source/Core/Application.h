#pragma once
#include "Actor.h"

class Material;
class Shader;
class RenderTexture;
class RenderTexture2D;
class StaticMeshActor;
class StaticMesh;
class FlyCamera;
class PointLightActor;

class Application : public Actor
{    
public:
    IMPLEMENT_CONSTRUCTOR(Application, Actor);
    
    virtual void Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void Shutdown() override;

    Material* Mat;
    std::vector<StaticMeshActor*> Meshes;
    Actor* RootActor;
    FlyCamera* FlyCam;
    PointLightActor* Light;
    float Angle = 0.0f;

    unsigned int NumLights = 100;
    StaticMesh* SphereMesh;
};
