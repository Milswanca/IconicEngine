#include "StaticMeshActor.h"
#include "StaticMeshComponent.h"

StaticMeshActor::StaticMeshActor(Object* NewOuter) : Actor(NewOuter)
{
    StaticMeshComp = AddComponent<StaticMeshComponent>();
    StaticMeshComp->AttachTo(GetRootComponent());
}

StaticMeshActor::~StaticMeshActor()
{
    
}

void StaticMeshActor::SetMesh(StaticMesh* NewMesh)
{
    GetStaticMeshComponent()->SetMesh(NewMesh);
}

StaticMesh* StaticMeshActor::GetMesh() const
{
    return GetStaticMeshComponent()->GetMesh();
}

void StaticMeshActor::Draw(const glm::vec3& ViewPos, const glm::mat4& View, const glm::mat4& Projection)
{
    GetStaticMeshComponent()->Draw(ViewPos, View, Projection);
}

StaticMeshComponent* StaticMeshActor::GetStaticMeshComponent() const
{
    return StaticMeshComp;
}
