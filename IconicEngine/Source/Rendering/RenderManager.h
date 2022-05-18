#pragma once
#include <vector>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "GBuffer.h"
#include "Core/Object.h"

class Shader;
class IDrawable;
class CameraComponent;
class RenderTexture;
class UniformBufferObject;
class Texture;
class Material;
class StaticMesh;

class RenderManager : public Object
{
public:
    enum class DrawMode
    {
        Forward,
        Deferred
    };

    enum class DrawOutputTarget
    {
        FinalColor,
        Albedo,
        Position,
        Normal,
        Ambient,
        Spec,
        Composited
    };

    struct CameraBufferData
    {
        glm::mat4 gProjectionView;
        glm::vec3 gEyePosition;
    };

    static StaticMesh* QuadMesh;
    static UniformBufferObject* CameraBuffer;
    static CameraBufferData CameraData;
    
public:
    IMPLEMENT_CONSTRUCTOR(RenderManager, Object);

    virtual void PostInit();
    virtual void Shutdown() override;

	void SetDrawMode(DrawMode NewDrawMode);
	void SetDrawOutputTarget(DrawOutputTarget Target);
    void OverrideGBuffer(GBuffer* NewGBuffer);

    void BindMaterial(Material* Mat);
    void BindTexture(Texture* Tex, unsigned int Index);
    void BindFramebuffer(RenderTexture* FB);
    void BindUniformBuffer(UniformBufferObject* Buf, unsigned int Index);

    Material* GetMaterial() const;
    Texture* GetTexture(unsigned int Index) const;
    RenderTexture* GetFramebuffer() const;
    UniformBufferObject* GetUniformBuffer(unsigned int Index) const;

    void Render();
    void RenderMesh(CameraComponent* Camera, const glm::mat4& Model, Material* Mat, StaticMesh* Mesh);

    void RenderScene(CameraComponent* Camera);
    void RenderScene(Shader* Shad, CameraComponent* Camera);

    void RegisterDrawable(IDrawable* Drawable);
    void DeregisterDrawable(IDrawable* Drawable);

    void SetMainCamera(CameraComponent* NewMainCamera);

private:
    unsigned int BoundProgramID = 0;
    Material* BoundMat;
    Texture* BoundTextures[32];
    UniformBufferObject* BoundBuffers[32];
    RenderTexture* BoundFramebuffer;
    CameraComponent* MainCamera;

    Material* OutputTargetMat;
    DrawOutputTarget OutputTarget;
    GBuffer* CurrentGBuffer;

    DrawMode CurrentDrawMode;

    //TODO: Dont use a dynamic array here we dont have to
    std::vector<IDrawable*> Drawables;
};

