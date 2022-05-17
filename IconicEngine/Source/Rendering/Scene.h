#pragma once
#include <vector>

#include "Utils/IndexArray.h"
#include "Assets/AssetTypes.h"
#include "Core/Object.h"

class Component;

static const unsigned short MAX_SCENE_NODES = 65534;
static const unsigned short INVALID_SCENE_NODE = 65535;

class Scene : public Object
{
    struct SceneNode
    {
    public:
        unsigned short NodeID;
        unsigned short ParentNode;
        std::vector<unsigned short> ChildrenNodes;
        
        ASSET_HANDLE Asset;
    };
    
public:
    IMPLEMENT_CONSTRUCTOR(Scene, Object);
    
    virtual void Init() override;
    virtual void Shutdown() override;
    
    unsigned short AddNode(unsigned short ParentNode, ASSET_HANDLE Asset);
    void RemoveNode(unsigned short Node);
    void SetParent(unsigned short NodeID, unsigned short NewParentID);

    unsigned short GetParent(unsigned short NodeID) const;
    unsigned short GetChild(unsigned short NodeID, unsigned short Index) const;
    unsigned short GetNumChildren(unsigned short NodeID) const;
    ASSET_HANDLE GetAsset(unsigned short NodeID) const;

    template<typename T, typename O>
    void ForEachChild_DepthFirst(unsigned short NodeID, O* Object, void(T::*Func)(unsigned short))
    {
        for(unsigned short Child : SceneNodes[NodeID]->ChildrenNodes)
        {
            ForEachChild_DepthFirst(Child, Object, Func);
        }

        (Object->*Func)(NodeID);
    }

    template<typename T, typename O>
    void ForEachChild_BredthFirst(unsigned short NodeID, O* Object, void(T::*Func)(unsigned short))
    {
        (Object->*Func)(NodeID);
        
        for(unsigned short Child : SceneNodes[NodeID]->ChildrenNodes)
        {
            ForEachChild_BredthFirst(Child, Object, Func);
        }
    }

private:
    void RemoveNodeInternal(unsigned short NodeID);
    
    SceneNode* RootNode;
    SceneNode** SceneNodes;
    Component** Components;
    unsigned short NumSceneNodes;
    IndexArray<unsigned short>* NodeIDs;
};