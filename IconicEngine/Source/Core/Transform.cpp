#include "Transform.h"

#include <glm/ext/matrix_transform.hpp>

Transform::Transform()
{
    Position = glm::vec3(0.0f, 0.0f, .0f);
    Rotation = glm::identity<glm::quat>();
    Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    RelativeTransform = glm::identity<glm::mat4>();
    LocalToWorld = glm::identity<glm::mat4>();
    WorldToLocal = glm::identity<glm::mat4>();

    Parent = nullptr;
}

void Transform::SetLocalPosition(const glm::vec3& LocalPosition)
{
    Position = LocalPosition;
    ApplyTransformation();
}

void Transform::SetLocalRotation(const glm::quat& LocalRotation)
{
    Rotation = LocalRotation;
    ApplyTransformation();
}

void Transform::SetLocalScale(const glm::vec3& LocalScale)
{
    Scale = LocalScale;
    ApplyTransformation();
}

void Transform::TranslateLocal(const glm::vec3& Translation)
{
	Position += GetRotation() * Translation;
	ApplyTransformation();
}

void Transform::TranslateWorld(const glm::vec3& Translation)
{
	Position += Translation;
    ApplyTransformation();
}

void Transform::Rotate(float Degrees, const glm::vec3& Axis)
{
    Rotation = glm::angleAxis(glm::radians(Degrees), Axis) * Rotation;
    ApplyTransformation();
}

void Transform::Rotate(const glm::quat& Quaternion)
{
	Rotation = Quaternion * Rotation;
	ApplyTransformation();
}

void Transform::Rotate(const glm::vec3& Euler)
{
	Rotation = glm::quat(glm::vec3(glm::radians(Euler.x), glm::radians(Euler.y), glm::radians(Euler.z))) * Rotation;
	ApplyTransformation();
}

glm::vec3 Transform::GetLocalPosition() const
{
    return Position;
}

glm::quat Transform::GetLocalRotation() const
{
    return Rotation;
}

glm::vec3 Transform::GetLocalScale() const
{
    return Scale;
}

glm::vec3 Transform::GetPosition() const
{
    return LocalToWorld * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::quat Transform::GetRotation() const
{
    return glm::toQuat(LocalToWorld);
}

glm::vec3 Transform::GetForward() const
{
    return LocalToWorld * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
}

glm::vec3 Transform::GetRight() const
{
    return LocalToWorld * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
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
    glm::mat4 T = glm::translate(glm::identity<glm::mat4>(), Position);
    glm::mat4 R = glm::toMat4(Rotation);
    glm::mat4 S = glm::scale(glm::identity<glm::mat4>(), Scale);

    RelativeTransform = T * R * S;
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