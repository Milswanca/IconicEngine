#pragma once
#include "Component.h"
#include "Engine.h"
#include "LookUpTable.h"
#include "Object.h"

static const unsigned short MAX_COMPONENTS = 65534;

class Actor : public Object
{
public:
    Actor(Object* Outer);
    virtual ~Actor();

    virtual void Spawned();
    virtual void Update(const float DeltaTime);
    virtual void RemovedFromWorld();

    void SetLocalPosition(const glm::vec3& Position);
    void SetLocalRotation(const glm::quat& Rotation);
    void SetPosition(const glm::vec3& Position);
    void SetRotation(const glm::quat& Rotation);
    
    glm::vec3 GetLocalPosition() const;
    glm::quat GetLocalRotation() const;
    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;

    void GetLocalToWorld(glm::mat4& OutLocalToWorld) const;
    void GetWorldToLocal(glm::mat4& OutWorldToLocal) const;
    
    template<typename T>
    T* AddComponent()
    {         
        T* Component = Engine::Get()->CreateObject<T>(this);
        Components->Add(Component);
        Component->Owner = this;
        return Component;
    }

    void RemoveComponent(Component* Component);

    Component* GetRootComponent() const;

private:
    LookUpTable<Component, unsigned short>* Components;
    Component* RootComponent;
    
    friend class World;
};
