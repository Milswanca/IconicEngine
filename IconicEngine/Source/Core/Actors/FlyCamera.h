#pragma once
#include "Core/Components/CameraComponent.h"
#include "Core/Actor.h"

class FlyCamera : public Actor
{
public:
    IMPLEMENT_CONSTRUCTOR(FlyCamera, Actor);

    virtual void Init() override;
    virtual void Shutdown() override;
    virtual void Update(const float DeltaTime) override;

    void SetFOV(float NewFOV);
    void SetAspect(float NewAspect);
    void SetNear(float NewNear);
    void SetFar(float NewFar);

    glm::mat4 GetProjection() const;
    glm::mat4 GetView() const;
    glm::mat4 GetProjectionView() const;

    CameraComponent* GetCameraComponent() const;

private:
    CameraComponent* CameraComp;
    float TurnSpeed = 180.0f;
    float MoveSpeed = 200.0f;
};
