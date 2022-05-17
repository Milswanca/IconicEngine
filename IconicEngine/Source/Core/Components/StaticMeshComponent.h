#pragma once
#include "Component.h"
#include "Rendering/StaticMesh.h"

class Material;
class Shader;

class StaticMeshComponent : public Component, public IDrawable
{
public:
    IMPLEMENT_CONSTRUCTOR(StaticMeshComponent, Component);

    virtual void Init() override;
    virtual void Shutdown() override;
    
    void SetMesh(StaticMesh* NewMesh);
    StaticMesh* GetMesh() const;

    virtual void Draw(Shader* Shad) override;

private:
    StaticMesh* Mesh;
};