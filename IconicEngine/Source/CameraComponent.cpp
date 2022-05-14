#include "CameraComponent.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

CameraComponent::CameraComponent(Object* NewOuter) : Component(NewOuter)
{
    Near = 0.3f;
    Far = 100000.0f;
    FOV = 45.0f;
    Aspect = 600.0f / 400.0f;
    RefreshProjectionView();
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(const float DeltaTime)
{
    Component::Update(DeltaTime);
    RefreshProjectionView();
}

void CameraComponent::SetFOV(float NewFOV)
{
    FOV = NewFOV;
    RefreshProjectionView();
}

void CameraComponent::SetNear(float NewNear)
{
    Near = NewNear;
    RefreshProjectionView();
}

void CameraComponent::SetFar(float NewFar)
{
    Far = NewFar;
    RefreshProjectionView();
}

void CameraComponent::SetAspect(float NewAspect)
{
    Aspect = NewAspect;
    RefreshProjectionView();
}

void CameraComponent::RefreshProjectionView()
{
    Projection = glm::perspective(FOV, Aspect, Near, Far);
    glm::vec3 Forward = GetForward();
    glm::vec3 Position = GetPosition();
    glm::vec3 Up = GetUp();
    View = glm::lookAt(GetPosition(), GetPosition() + (GetForward() * 100.0f), GetUp());
    ProjectionView = Projection * View;
}

glm::mat4 CameraComponent::GetProjection() const
{
    return Projection;
}

glm::mat4 CameraComponent::GetView() const
{
    return View;
}

glm::mat4 CameraComponent::GetProjectionView() const
{
    return ProjectionView;
}