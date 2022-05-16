#include "Component.h"
#include "Core/Actor.h"
#include "Transform.h"

void Component::Init()
{
    Object::Init();

    Trans = new Transform();
}

void Component::Shutdown()
{
    Object::Shutdown();

    delete Trans;
}

void Component::Update(const float DeltaTime)
{
    for(size_t i = 0; i < AttachChildren.size(); ++i)
    {
        AttachChildren[i]->Update(DeltaTime);
    }
}

Actor* Component::GetOwner() const
{
    return Owner;
}

void Component::AttachTo(Component* Other)
{
    if(AttachParent != nullptr)
    {
        AttachParent->RemoveChild(this);
    }

    Trans->SetParent(Other != nullptr ? Other->Trans : nullptr);
    AttachParent = Other;

    if(AttachParent)
    {
        AttachParent->AddChild(this);
    }
}

void Component::AddChild(Component* Child)
{
    AttachChildren.push_back(Child);
}

void Component::RemoveChild(Component* Child)
{
    remove(AttachChildren.begin(), AttachChildren.end(), Child);
}

void Component::SetLocalPosition(const glm::vec3& Position)
{
    Trans->SetLocalPosition(Position);
}

void Component::SetLocalRotation(const glm::quat& Rotation)
{
    Trans->SetLocalRotation(Rotation);
}

void Component::SetPosition(const glm::vec3& Position)
{
    Trans->SetPosition(Position);
}

void Component::SetRotation(const glm::quat& Rotation)
{
    Trans->SetRotation(Rotation);
}

glm::vec3 Component::GetLocalPosition() const
{
    return Trans->GetLocalPosition();
}

glm::quat Component::GetLocalRotation() const
{
    return Trans->GetLocalRotation();
}

glm::vec3 Component::GetPosition() const
{
    return Trans->GetPosition();
}

glm::quat Component::GetRotation() const
{
    return Trans->GetRotation();
}

glm::vec3 Component::GetForward() const
{
    return Trans->GetForward();
}

glm::vec3 Component::GetRight() const
{
    return Trans->GetRight();
}

glm::vec3 Component::GetUp() const
{
    return Trans->GetUp();
}

void Component::GetLocalToWorld(glm::mat4& OutLocalToWorld) const
{
    Trans->GetLocalToWorld(OutLocalToWorld);
}

void Component::GetWorldToLocal(glm::mat4& OutWorldToLocal) const
{
    Trans->GetWorldToLocal(OutWorldToLocal);
}
