#pragma once
#include "Core/Actor.h"

class Material;
class Shader;
class StaticMesh;
class StaticMeshComponent;

class StaticMeshActor : public Actor
{
public:
    StaticMeshActor(Object* NewOuter);
    ~StaticMeshActor();
    
    void SetMesh(StaticMesh* NewMesh);
    StaticMesh* GetMesh() const;

    void Draw(const glm::vec3& ViewPos, const glm::mat4& View, const glm::mat4& Projection);
    
    StaticMeshComponent* GetStaticMeshComponent() const;
    
private:
    StaticMeshComponent* StaticMeshComp;
};
