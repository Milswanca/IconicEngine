#include "FlyCamera.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/detail/type_quat.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>

void FlyCamera::Init()
{
    Actor::Init();

    CameraComp = AddComponent<CameraComponent>();
    CameraComp->AttachTo(GetRootComponent());
}

void FlyCamera::Shutdown()
{
    Actor::Shutdown();
}

void FlyCamera::Update(const float DeltaTime)
{
    Actor::Update(DeltaTime);

    glm::vec3 Position = GetPosition();
    glm::quat Rotation = GetRotation();
    if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
    {
        Position += GetForward() * DeltaTime * MoveSpeed;
    }
    if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
    {
        Position -= GetForward() * DeltaTime * MoveSpeed;
    }
    if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
    {
        Position -= GetRight() * DeltaTime * MoveSpeed;
    }
    if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += GetRight() * DeltaTime * MoveSpeed;
    }
    if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Position += GetUp() * DeltaTime * MoveSpeed;
    }
    if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Position -= GetUp() * DeltaTime * MoveSpeed;
    }
    if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_Q) == GLFW_PRESS)
    {
        Rotation = glm::angleAxis(glm::radians(TurnSpeed * DeltaTime), GetUp()) * Rotation;
    }
    if (glfwGetKey(Engine::Get()->GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
    {
        Rotation = glm::angleAxis(-glm::radians(TurnSpeed * DeltaTime), GetUp()) * Rotation;
    }

    SetPosition(Position);
    SetLocalRotation(Rotation);
}

void FlyCamera::SetFOV(float NewFOV)
{
    CameraComp->SetFOV(NewFOV);
}

void FlyCamera::SetAspect(float NewAspect)
{
    CameraComp->SetAspect(NewAspect);
}

void FlyCamera::SetNear(float NewNear)
{
    CameraComp->SetNear(NewNear);
}

void FlyCamera::SetFar(float NewFar)
{
    CameraComp->SetFar(NewFar);
}

glm::mat4 FlyCamera::GetProjection() const
{
    return CameraComp->GetProjection();
}

glm::mat4 FlyCamera::GetView() const
{
    return CameraComp->GetView();
}

glm::mat4 FlyCamera::GetProjectionView() const
{
    return CameraComp->GetProjectionView();
}

CameraComponent* FlyCamera::GetCameraComponent() const
{
    return CameraComp;
}
