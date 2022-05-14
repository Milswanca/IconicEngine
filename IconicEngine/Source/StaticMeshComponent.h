#pragma once
#include "Component.h"
#include "StaticMesh.h"

class Material;
class Shader;

class StaticMeshComponent : public Component
{
public:
    StaticMeshComponent(Object* NewOuter);
    ~StaticMeshComponent();
    
    void SetMesh(StaticMesh* NewMesh);
    StaticMesh* GetMesh() const;

    void Draw(const glm::vec3& ViewPos, const glm::mat4& View, const glm::mat4& Projection);

private:
    StaticMesh* Mesh;
};