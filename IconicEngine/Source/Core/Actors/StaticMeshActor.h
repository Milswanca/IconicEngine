#pragma once
#include "Core/Actor.h"

class Material;
class Shader;
class StaticMesh;
class StaticMeshComponent;

class StaticMeshActor : public Actor
{
public:
    IMPLEMENT_CONSTRUCTOR(StaticMeshActor, Actor);

    virtual void Init() override;
    
    void SetMesh(StaticMesh* NewMesh);
    StaticMesh* GetMesh() const;

    void Draw();
    
    StaticMeshComponent* GetStaticMeshComponent() const;
    
private:
    StaticMeshComponent* StaticMeshComp;
};
