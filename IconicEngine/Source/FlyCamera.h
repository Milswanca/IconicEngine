#pragma once
#include "CameraComponent.h"
#include "Core/Actor.h"

class FlyCamera : public Actor
{
public:
    FlyCamera(Object* NewOuter);
    virtual ~FlyCamera();

    virtual void Update(const float DeltaTime) override;

    void SetFOV(float NewFOV);
    void SetAspect(float NewAspect);
    void SetNear(float NewNear);
    void SetFar(float NewFar);

    glm::mat4 GetProjection() const;
    glm::mat4 GetView() const;
    glm::mat4 GetProjectionView() const;

private:
    CameraComponent* CameraComp;
    float TurnSpeed = 180.0f;
    float MoveSpeed = 200.0f;
};
