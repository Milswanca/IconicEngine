#include "Scene.h"

Scene::Scene(Object* NewOuter) : Object(NewOuter)
{
    SceneNodes = new SceneNode*[MAX_SCENE_NODES];
    Components = new Component*[MAX_SCENE_NODES];
    NodeIDs = new IndexArray<unsigned short>(MAX_SCENE_NODES);

    // Add Root Node
    AddNode(INVALID_SCENE_NODE, -1);
}

Scene::~Scene()
{
    RemoveNode(0);
    delete[] SceneNodes;
    delete[] Components;
    delete NodeIDs;
}

unsigned short Scene::AddNode(unsigned short ParentNode, ASSET_HANDLE Asset)
{
    unsigned short NewID = NodeIDs->Add();
    
    SceneNode* NewNode = new SceneNode();
    NewNode->NodeID = NewID;
    NewNode->ParentNode = ParentNode;
    NewNode->Asset = Asset;

    if(ParentNode == INVALID_SCENE_NODE && RootNode == nullptr)
    {
        RootNode = NewNode;
    }

    ParentNode = ParentNode == INVALID_SCENE_NODE ? RootNode->NodeID : ParentNode;
    SceneNodes[ParentNode]->ChildrenNodes.push_back(NewID);
    
    return NewID;
}

void Scene::RemoveNodeInternal(unsigned short CurrentNodeID)
{
    delete SceneNodes[CurrentNodeID];
    SceneNodes[CurrentNodeID] = nullptr;
}

void Scene::RemoveNode(unsigned short Node)
{
    ForEachChild_DepthFirst(Node, this, &Scene::RemoveNodeInternal);
    SetParent(Node, INVALID_SCENE_NODE);
}

void Scene::SetParent(unsigned short NodeID, unsigned short NewParentID)
{
    if(SceneNodes[NodeID]->ParentNode == NewParentID)
        return;
    
    if(SceneNodes[NodeID]->ParentNode != INVALID_SCENE_NODE)
    {
        std::vector<unsigned short>* ChildrenNodes = &SceneNodes[NodeID]->ChildrenNodes;
        std::remove(ChildrenNodes->begin(), ChildrenNodes->end(), NodeID);
    }

    if(NewParentID != INVALID_SCENE_NODE)
    {
        SceneNodes[NewParentID]->ChildrenNodes.push_back(NodeID);
    }
    
    SceneNodes[NodeID]->ParentNode = NewParentID;
}

unsigned short Scene::GetParent(unsigned short NodeID) const
{
    return SceneNodes[NodeID]->ParentNode;
}

unsigned short Scene::GetChild(unsigned short NodeID, unsigned short Index) const
{
    return SceneNodes[NodeID]->ChildrenNodes[Index];
}

unsigned short Scene::GetNumChildren(unsigned short NodeID) const
{
    return SceneNodes[NodeID]->ChildrenNodes.size();
}

ASSET_HANDLE Scene::GetAsset(unsigned short NodeID) const
{
    return SceneNodes[NodeID]->Asset;
}