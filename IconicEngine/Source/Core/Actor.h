#pragma once
#include "Core/Components/Component.h"
#include "Engine.h"
#include "Utils/LookUpTable.h"
#include "Object.h"

static const unsigned short MAX_COMPONENTS = 65534;

class Actor : public Object
{
public:
    IMPLEMENT_CONSTRUCTOR(Actor, Object);

    virtual void Init() override;
    virtual void Shutdown() override;

    virtual void Update(const float DeltaTime);

    void SetLocalPosition(const glm::vec3& Position);
    void SetLocalRotation(const glm::quat& Rotation);
	void SetLocalScale(const glm::vec3& LocalScale);

	void TranslateLocal(const glm::vec3& Translation);
	void TranslateWorld(const glm::vec3& Translation);

	void Rotate(float Degrees, const glm::vec3& Axis);
	void Rotate(const glm::quat& Quaternion);
	void Rotate(const glm::vec3& Euler);

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
