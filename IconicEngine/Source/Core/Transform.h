#pragma once
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform
{
    Transform();
    
    void SetLocalPosition(const glm::vec3& LocalPosition);
    void SetLocalRotation(const glm::quat& LocalRotation);

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

    void SetParent(Transform* NewParent);
    Transform* GetParent() const;

private:
    void ApplyTransformation();
    void RecalculateSpaceTransforms();
    
    void AddChild(Transform* NewChild);
    void RemoveChild(Transform* RemChild);
    
    glm::mat4 LocalToWorld;
    glm::mat4 WorldToLocal;

    glm::mat4 RelativeTransform;
    glm::mat4 Rotation;
    glm::mat4 Translation;

    Transform* Parent;
    std::vector<Transform*> Children;
};