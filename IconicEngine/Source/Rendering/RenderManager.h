#pragma once
#include <vector>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "GBuffer.h"
#include "Core/Object.h"
#include "Core/Components/PointLightComponent.h"
#include "Core/Components/DirectionalLightComponent.h"

class LightComponent;
class PointLightComponent;
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
	static const unsigned int MaxDirectionalLights = 8;
	static const unsigned int MaxPointLights = 100;

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
        Composited,
        ShadowMap
    };

    struct CameraBufferData
    {
        glm::mat4 gProjectionView;
        glm::vec4 gEyePosition;
        float gNear = 0.3f;
        float gFar = 10000.0f;
    };

    struct LightBufferData
    {
        PointLightComponent::Data PointLights[MaxPointLights];
        DirectionalLightComponent::Data DirectionalLights[MaxDirectionalLights];

		int NumPointLights;
		int NumDirectionalLights;
    };

    static StaticMesh* QuadMesh;

    static UniformBufferObject* CameraBuffer;
    static CameraBufferData CameraData;

    static UniformBufferObject* LightBuffer;
    static LightBufferData LightData;

    static Shader* ShadowMapShader;

public:
    IMPLEMENT_CONSTRUCTOR(RenderManager, Object);

    virtual void PostInit();
    virtual void Shutdown() override;

	void SetDrawMode(DrawMode NewDrawMode);
	void SetDrawOutputTarget(DrawOutputTarget Target);
    void OverrideGBuffer(GBuffer* NewGBuffer);

    void RegisterLight(LightComponent* Light);
    void DeregisterLight(LightComponent* Light);

    void BindMaterial(Material* Mat);
    void BindTexture(Texture* Tex, unsigned int Index);
    void BindFramebuffer(RenderTexture* FB);
    void BindUniformBuffer(UniformBufferObject* Buf, unsigned int Index);

    Material* GetMaterial() const;
    Texture* GetTexture(unsigned int Index) const;
    RenderTexture* GetFramebuffer() const;
    UniformBufferObject* GetUniformBuffer(unsigned int Index) const;

    void GenerateShadowMap(LightComponent* Light);

    void Render();
    void RenderMesh(const glm::mat4& ProjectionView, const glm::mat4& Model, Material* Mat, StaticMesh* Mesh);

    void RenderScene(const glm::mat4& ProjectionView);
    void RenderScene(Shader* Shad, const glm::mat4& ProjectionView);

    void RegisterDrawable(IDrawable* Drawable);
    void DeregisterDrawable(IDrawable* Drawable);

    void SetMainCamera(CameraComponent* NewMainCamera);

private:
    void BufferCameraData(const glm::mat4& ProjectionView);
    void BufferLightData();

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
    std::vector<LightComponent*> Lights;
};

