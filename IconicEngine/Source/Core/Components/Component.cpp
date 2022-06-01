#include "Component.h"
#include "Core/Actor.h"
#include "Core/Transform.h"

void Component::Init()
{
    Object::Init();

    Trans = new Transform();
    Owner = dynamic_cast<Actor*>(GetOuter());
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

void Component::SetLocalScale(const glm::vec3& LocalScale)
{
    Trans->SetLocalScale(LocalScale);
}

void Component::TranslateLocal(const glm::vec3& Translation)
{
    Trans->TranslateLocal(Translation);
}

void Component::TranslateWorld(const glm::vec3& Translation)
{
    Trans->TranslateWorld(Translation);
}

void Component::Rotate(float Degrees, const glm::vec3& Axis)
{
    Trans->Rotate(Degrees, Axis);
}

void Component::Rotate(const glm::quat& Quaternion)
{
    Trans->Rotate(Quaternion);
}

void Component::Rotate(const glm::vec3& Euler)
{
    Trans->Rotate(Euler);
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
