#pragma once
#include "Component.h"

class Scene;

class SceneComponent : public Component
{
public:
    IMPLEMENT_CONSTRUCTOR(SceneComponent, Component);

    virtual void Init() override;
    virtual void Shutdown() override;

    void SetScene(Scene* NewScene);

private:
    void RefreshComponentTree(unsigned short Branch);
    void DeleteComponentTree(unsigned short Branch);

    void SpawnComponentTreeNode(unsigned short Node);
    void DeleteComponentTreeNode(unsigned short Node);

    Scene* MyScene;
    Component** Components;
};
