#include "StaticMeshComponent.h"

#include "Material.h"
#include "Shader.h"

StaticMeshComponent::StaticMeshComponent(Object* NewOuter) : Component(NewOuter)
{
    
}

StaticMeshComponent::~StaticMeshComponent()
{
    
}

void StaticMeshComponent::Draw(const glm::vec3& ViewPos, const glm::mat4& View, const glm::mat4& Projection)
{
    glm::mat4 LocalToWorld;
    GetLocalToWorld(LocalToWorld);
    
    if(Mesh)
    {
        Mesh->Draw(ViewPos, View, Projection, LocalToWorld);
    }
}

void StaticMeshComponent::SetMesh(StaticMesh* NewMesh)
{
    Mesh = NewMesh;
}

StaticMesh* StaticMeshComponent::GetMesh() const
{
    return Mesh;
}