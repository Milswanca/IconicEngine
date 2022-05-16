#pragma once
#include <vector>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include "Core/Object.h"

class IDrawable;
class CameraComponent;
class RenderTexture;
class UniformBufferObject;
class Texture;
class Material;

class RenderManager : public Object
{
public:
    struct CameraBufferData
    {
        glm::mat4 gProjectionView;
        glm::vec3 gEyePosition;
    };
    
    static UniformBufferObject* CameraBuffer;
    static CameraBufferData CameraData;
    
public:
    IMPLEMENT_CONSTRUCTOR(RenderManager, Object);

    virtual void Init() override;
    virtual void Shutdown() override;

    void BindMaterial(Material* Mat);
    void BindTexture(Texture* Tex, unsigned int Index);
    void BindFramebuffer(RenderTexture* FB);
    void BindUniformBuffer(UniformBufferObject* Buf, unsigned int Index);

    Material* GetMaterial() const;
    Texture* GetTexture(unsigned int Index) const;
    RenderTexture* GetFramebuffer() const;
    UniformBufferObject* GetUniformBuffer(unsigned int Index) const;

    void RenderScene(CameraComponent* Camera);

    void RegisterDrawable(IDrawable* Drawable);
    void DeregisterDrawable(IDrawable* Drawable);

private:
    unsigned int BoundProgramID = 0;
    Material* BoundMat;
    Texture* BoundTextures[32];
    UniformBufferObject* BoundBuffers[32];
    RenderTexture* BoundFramebuffer;

    //TODO: Dont use a dynamic array here we dont have to
    std::vector<IDrawable*> Drawables;
};

