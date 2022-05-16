#include "SceneComponent.h"

#include "Scene.h"
#include "Assets/AssetManager.h"
#include "Core/Actor.h"
#include "Core/Engine.h"

void SceneComponent::Init()
{
    Component::Init();
    Components = new Component*[MAX_SCENE_NODES];
}

void SceneComponent::Shutdown()
{
    Component::Shutdown();
    delete[] Components;
}

void SceneComponent::SetScene(Scene* NewScene)
{
    DeleteComponentTree(0);
    MyScene = NewScene;
    RefreshComponentTree(0);
}

void SceneComponent::RefreshComponentTree(unsigned short Branch)
{
    if(!MyScene)
        return;
    
    DeleteComponentTree(Branch);
    MyScene->ForEachChild_BredthFirst(Branch, this, &SceneComponent::SpawnComponentTreeNode);
}

void SceneComponent::DeleteComponentTree(unsigned short Branch)
{    
    if(MyScene == nullptr)
        return;

    MyScene->ForEachChild_DepthFirst(Branch, this, &SceneComponent::DeleteComponentTreeNode);
}

void SceneComponent::SpawnComponentTreeNode(unsigned short Node)
{
    unsigned short Parent = MyScene->GetParent(Node);
    ASSET_HANDLE AssetHandle = MyScene->GetAsset(Node);
    AssetResource* Asset = Engine::Get()->GetAssetManager()->GetAssetObject(AssetHandle);

    Component* Comp = nullptr;
    if(Asset)
    {
        Comp = Asset->SpawnComponentForAsset();
        Asset->ApplyToComponent(Comp);
    }
    else
    {
        Comp = GetOwner()->AddComponent<Component>();
    }

    Component* AttachParent = Parent != INVALID_SCENE_NODE ? Components[Parent] : this;
    Comp->AttachTo(AttachParent);
    Components[Node] = Comp;
}

void SceneComponent::DeleteComponentTreeNode(unsigned short Node)
{
    Components[Node]->Destroy();
    Components[Node] = nullptr;
}
