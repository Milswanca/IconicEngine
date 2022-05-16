#include "StaticMeshComponent.h"

#include "Material.h"
#include "RenderManager.h"
#include "Shader.h"

void StaticMeshComponent::Init()
{
    Component::Init();

    GetRenderManager()->RegisterDrawable(this);
}

void StaticMeshComponent::Shutdown()
{
    Component::Shutdown();

    GetRenderManager()->DeregisterDrawable(this);
}

void StaticMeshComponent::Draw()
{
    glm::mat4 LocalToWorld;
    GetLocalToWorld(LocalToWorld);
    
    if(Mesh)
    {
        Mesh->Draw(LocalToWorld);
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