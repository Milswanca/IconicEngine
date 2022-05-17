#pragma once
#include "Core/Components/Component.h"
#include <glm/ext/matrix_float4x4.hpp>

class CameraComponent : public Component
{
public:
    IMPLEMENT_CONSTRUCTOR(CameraComponent, Component);
    
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

private:
    void RefreshProjectionView();
    
    float FOV;
    float Near;
    float Far;
    float Aspect;

    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 ProjectionView;
};
