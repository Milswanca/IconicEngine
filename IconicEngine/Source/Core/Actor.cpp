#include "Actor.h"

#include <glm/detail/type_quat.hpp>

#include "Core/Components/Component.h"
#include "World.h"

void Actor::Init()
{
    Object::Init();

    Components = new LookUpTable<Component, unsigned short>(MAX_COMPONENTS);
    RootComponent = AddComponent<Component>();
}

void Actor::Shutdown()
{
    Object::Shutdown();

    delete Components;
}

void Actor::Update(const float DeltaTime)
{
    GetRootComponent()->Update(DeltaTime);
}

void Actor::SetLocalPosition(const glm::vec3& Position)
{
    GetRootComponent()->SetLocalPosition(Position);
}

void Actor::SetLocalRotation(const glm::quat& Rotation)
{
    GetRootComponent()->SetLocalRotation(Rotation);
}

void Actor::SetPosition(const glm::vec3& Position)
{
    GetRootComponent()->SetPosition(Position);
}

void Actor::SetRotation(const glm::quat& Rotation)
{
    GetRootComponent()->SetRotation(Rotation);
}

glm::vec3 Actor::GetLocalPosition() const
{
    return GetRootComponent()->GetLocalPosition();
}

glm::quat Actor::GetLocalRotation() const
{
    return GetRootComponent()->GetLocalRotation();
}

glm::vec3 Actor::GetPosition() const
{
    return GetRootComponent()->GetPosition();
}

glm::quat Actor::GetRotation() const
{
    return GetRootComponent()->GetRotation();
}

glm::vec3 Actor::GetForward() const
{
    return GetRootComponent()->GetForward();
}

glm::vec3 Actor::GetRight() const
{
    return GetRootComponent()->GetRight();
}

glm::vec3 Actor::GetUp() const
{
    return GetRootComponent()->GetUp();
}

void Actor::GetLocalToWorld(glm::mat4& OutLocalToWorld) const
{
    GetRootComponent()->GetLocalToWorld(OutLocalToWorld);
}

void Actor::GetWorldToLocal(glm::mat4& OutWorldToLocal) const
{
    GetRootComponent()->GetWorldToLocal(OutWorldToLocal);
}

void Actor::RemoveComponent(Component* Component)
{
    Components->Remove(Component);
    Component->Destroy();
}

Component* Actor::GetRootComponent() const
{
    return RootComponent;
}
