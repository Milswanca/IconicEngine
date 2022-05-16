#pragma once
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include "Core/Object.h"

class Actor;
struct Transform;

class Component : public Object
{
public:
    IMPLEMENT_CONSTRUCTOR(Component, Object);

    virtual void Init() override;
    virtual void Shutdown() override;
    
    virtual void Update(const float DeltaTime);

    Actor* GetOwner() const;

    void AttachTo(Component* Other);
    
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

private:
    void AddChild(Component* Child);
    void RemoveChild(Component* Child);

    Actor* Owner;
    Component* AttachParent;
    std::vector<Component*> AttachChildren;
    Transform* Trans;

    friend class Actor;
};
