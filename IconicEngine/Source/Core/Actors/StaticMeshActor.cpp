#include "StaticMeshActor.h"
#include "Core/Components/StaticMeshComponent.h"

void StaticMeshActor::Init()
{
    Actor::Init();

    StaticMeshComp = AddComponent<StaticMeshComponent>();
    StaticMeshComp->AttachTo(GetRootComponent());
}

void StaticMeshActor::SetMesh(StaticMesh* NewMesh)
{
    GetStaticMeshComponent()->SetMesh(NewMesh);
}

StaticMesh* StaticMeshActor::GetMesh() const
{
    return GetStaticMeshComponent()->GetMesh();
}

void StaticMeshActor::Draw()
{
    GetStaticMeshComponent()->Draw(nullptr);
}

StaticMeshComponent* StaticMeshActor::GetStaticMeshComponent() const
{
    return StaticMeshComp;
}
