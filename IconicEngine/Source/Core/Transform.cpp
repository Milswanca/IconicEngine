#include "Transform.h"

#include <glm/ext/matrix_transform.hpp>

Transform::Transform()
{
    Translation = glm::identity<glm::mat4>();
    Rotation = glm::identity<glm::mat4>();
    RelativeTransform = glm::identity<glm::mat4>();
    LocalToWorld = glm::identity<glm::mat4>();
    WorldToLocal = glm::identity<glm::mat4>();

    Parent = nullptr;
}

void Transform::SetLocalPosition(const glm::vec3& LocalPosition)
{
    Translation = glm::translate(glm::identity<glm::mat4>(), LocalPosition);
    ApplyTransformation();
}

void Transform::SetLocalRotation(const glm::quat& LocalRotation)
{
    Rotation = glm::toMat4(LocalRotation);
    ApplyTransformation();
}

void Transform::SetPosition(const glm::vec3& Position)
{
    glm::vec3 Delta = Position - GetPosition();
    SetLocalPosition(GetLocalPosition() + Delta);
}

void Transform::SetRotation(const glm::quat& Rotation)
{
    glm::quat Delta = GetRotation() * glm::inverse(Rotation);
    SetLocalRotation(GetLocalRotation() * Delta);
}

glm::vec3 Transform::GetLocalPosition() const
{
    return Translation[3];
}

glm::quat Transform::GetLocalRotation() const
{
    return glm::toQuat(Rotation);
}

glm::vec3 Transform::GetPosition() const
{
    return LocalToWorld[3];
}

glm::quat Transform::GetRotation() const
{
    return glm::quat_cast(LocalToWorld);
}

glm::vec3 Transform::GetForward() const
{
    return LocalToWorld * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
}

glm::vec3 Transform::GetRight() const
{
    return LocalToWorld * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
}

glm::vec3 Transform::GetUp() const
{
    return LocalToWorld * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
}

void Transform::GetLocalToWorld(glm::mat4& OutLocalToWorld) const
{
    OutLocalToWorld = LocalToWorld;
}

void Transform::GetWorldToLocal(glm::mat4& OutWorldToLocal) const
{
    OutWorldToLocal = WorldToLocal;
}

void Transform::SetParent(Transform* NewParent)
{
    if(Parent != nullptr)
    {
        Parent->RemoveChild(this);
    }

    Parent = NewParent;

    if(Parent)
    {
        Parent->AddChild(this);
    }

    ApplyTransformation();
}

Transform* Transform::GetParent() const
{
    return Parent;
}

void Transform::ApplyTransformation()
{
    //Calc Matrices here
    RelativeTransform = Translation * Rotation;
    RecalculateSpaceTransforms();
}

void Transform::RecalculateSpaceTransforms()
{
    LocalToWorld = Parent != nullptr ? Parent->LocalToWorld * RelativeTransform : RelativeTransform;
    WorldToLocal = glm::inverse(LocalToWorld);
    
    for(size_t i = 0; i < Children.size(); ++i)
    {
        Children[i]->ApplyTransformation();
    }
}

void Transform::AddChild(Transform* NewChild)
{
    Children.push_back(NewChild);
}

void Transform::RemoveChild(Transform* RemChild)
{
    remove(Children.begin(), Children.end(), RemChild);
}