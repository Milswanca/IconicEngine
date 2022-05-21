#include "RenderManager.h"

#include <glm/ext/matrix_transform.hpp>

#include "Core/Components/CameraComponent.h"
#include "IDrawable.h"
#include "Material.h"
#include "RenderTexture.h"
#include "Texture.h"
#include "UniformBufferObject.h"
#include "StaticMesh.h"

UniformBufferObject* RenderManager::CameraBuffer = nullptr;
RenderManager::CameraBufferData RenderManager::CameraData;

UniformBufferObject* RenderManager::LightBuffer = nullptr;
RenderManager::LightBufferData RenderManager::LightData;

StaticMesh* RenderManager::QuadMesh = nullptr;

void RenderManager::PostInit()
{
    StaticMesh::FCreateStaticMeshParams SMParams;
    SMParams.Positions = {
        glm::vec3(-1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f)
    };
    SMParams.UVs = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
    };
    SMParams.Triangles = { 
        { 0, 1, 2, 2, 1, 3 }
    };
    QuadMesh = StaticMesh::Create(this, SMParams);

    Shader* OutputTargetShader = CreateObject<Shader>(this);
	OutputTargetShader->SetShaderSource(ShaderTypes::Vertex, "Content\\Shaders\\DrawFullScreenVS.shader");
	OutputTargetShader->SetShaderSource(ShaderTypes::Fragment, "Content\\Shaders\\DrawFullScreenFS.shader");
    OutputTargetShader->Compile();

    OutputTargetMat = CreateObject<Material>(this);
    OutputTargetMat->SetShader(OutputTargetShader);
    
    CameraBuffer = UniformBufferObject::Create(this, sizeof(CameraBufferData));
	LightBuffer = UniformBufferObject::Create(this, sizeof(LightBufferData));

    CurrentGBuffer = CreateObject<GBufferDeferred>(this);

    SetDrawMode(DrawMode::Deferred);
    SetDrawOutputTarget(DrawOutputTarget::FinalColor);
}

void RenderManager::Shutdown()
{
    Object::Shutdown();
}

void RenderManager::SetDrawMode(DrawMode NewDrawMode)
{
    CurrentDrawMode = NewDrawMode;
}

void RenderManager::SetDrawOutputTarget(DrawOutputTarget Target)
{
    OutputTarget = Target;
}

void RenderManager::OverrideGBuffer(GBuffer* NewGBuffer)
{
    CurrentGBuffer = NewGBuffer;
}

void RenderManager::RegisterLight(LightComponent* Light)
{
    Lights.push_back(Light);
}

void RenderManager::DeregisterLight(LightComponent* Light)
{
    std::remove(Lights.begin(), Lights.end(), Light);
}

void RenderManager::BindMaterial(Material* Mat)
{
    if(BoundMat == Mat)
        return;

    if(BoundMat)
        BoundMat->MaterialUnbound();

    int NewProgramID = Mat != nullptr ? Mat->GetProgramID() : 0;
    glUseProgram(NewProgramID);
    BoundProgramID = NewProgramID;
    BoundMat = Mat;

    if (BoundMat)
        BoundMat->MaterialBound();
}

void RenderManager::BindTexture(Texture* Tex, unsigned Index)
{
    if(BoundTextures[Index] == Tex)
        return;

    if(BoundTextures[Index])
        BoundTextures[Index]->TextureUnbound(Index);
    
    glActiveTexture(GL_TEXTURE0 + Index);
    glBindTexture(GL_TEXTURE_2D, Tex != nullptr ? Tex->GetTextureID() : 0);
    BoundTextures[Index] = Tex;

    if(BoundTextures[Index])
        BoundTextures[Index]->TextureBound(Index);
}

void RenderManager::BindFramebuffer(RenderTexture* FB)
{
    if(BoundFramebuffer == FB)
        return;

    if(BoundFramebuffer)
        BoundFramebuffer->FramebufferUnbound();
    
    glBindFramebuffer(GL_FRAMEBUFFER, FB != nullptr ? FB->GetFBO() : 0);
    BoundFramebuffer = FB;

    if(BoundFramebuffer)
        BoundFramebuffer->FramebufferBound();
}

void RenderManager::BindUniformBuffer(UniformBufferObject* Buf, unsigned Index)
{
    if(BoundBuffers[Index] == Buf)
        return;

    if(BoundBuffers[Index])
        BoundBuffers[Index]->BufferUnbound(Index);

    glBindBuffer(GL_UNIFORM_BUFFER, Buf->GetUBO());
    glBindBufferRange(GL_UNIFORM_BUFFER, Index, Buf != nullptr ? Buf->GetUBO() : 0, 0, Buf != nullptr ? Buf->GetSize() : 0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    BoundBuffers[Index] = Buf;

    if(BoundBuffers[Index])
        BoundBuffers[Index]->BufferBound(Index);
}

Material* RenderManager::GetMaterial() const
{
    return BoundMat;
}

Texture* RenderManager::GetTexture(unsigned Index) const
{
    return BoundTextures[Index];
}

RenderTexture* RenderManager::GetFramebuffer() const
{
    return BoundFramebuffer;
}

UniformBufferObject* RenderManager::GetUniformBuffer(unsigned Index) const 
{
    return BoundBuffers[Index];
}

void RenderManager::Render()
{
    for(unsigned int i = 0; i < GBuffer::MAX_GBUFFER_PASSES; ++i)
    {
        GBuffer::GBufferPass* Pass = CurrentGBuffer->GetPassData(i);

        if(Pass)
        {
            CurrentGBuffer->PreRenderPass(i);

            switch(Pass->RenderType)
            {
            case GBuffer::GBufferRenderPassType::RenderScene:
                RenderScene(Pass->RenderShader, MainCamera);
                break;

            case GBuffer::GBufferRenderPassType::RenderQuad:
                RenderMesh(MainCamera, glm::identity<glm::mat4>(), Pass->RenderMaterial, QuadMesh);
                break;
            }

            CurrentGBuffer->PostRenderPass(i);
        }

        if (CurrentDrawMode == DrawMode::Deferred)
        {
            GBufferDeferred* DeferredBuffer = static_cast<GBufferDeferred*>(CurrentGBuffer);
			switch (OutputTarget)
			{
			case DrawOutputTarget::Albedo:
				OutputTargetMat->SetTexture("gTex_Output", DeferredBuffer->GetAlbedoTexture());
				break;
			case DrawOutputTarget::Ambient:
				OutputTargetMat->SetTexture("gTex_Output", DeferredBuffer->GetAmbientTexture());
				break;
			case DrawOutputTarget::Position:
				OutputTargetMat->SetTexture("gTex_Output", DeferredBuffer->GetPositionTexture());
				break;
			case DrawOutputTarget::Normal:
				OutputTargetMat->SetTexture("gTex_Output", DeferredBuffer->GetNormalTexture());
				break;
			case DrawOutputTarget::Spec:
				OutputTargetMat->SetTexture("gTex_Output", DeferredBuffer->GetSpecularTexture());
				break;
			case DrawOutputTarget::Composited:
				OutputTargetMat->SetTexture("gTex_Output", DeferredBuffer->GetCompositedTexture());
				break;
			case DrawOutputTarget::FinalColor:
				OutputTargetMat->SetTexture("gTex_Output", DeferredBuffer->GetFinalTexture());
				break;
			}
        }

        RenderMesh(MainCamera, glm::identity<glm::mat4>(), OutputTargetMat, QuadMesh);
    }
}

void RenderManager::RenderMesh(CameraComponent* Camera, const glm::mat4& Model, Material* Mat, StaticMesh* Mesh)
{
    BufferCameraData(Camera);
    BufferLightData();
	CameraBuffer->Bind(0);
    LightBuffer->Bind(1);

	if (Mesh)
	{
		glBindVertexArray(Mesh->GetMeshData()->VertexArrayObject);

		for (unsigned int i = 0; i < Mesh->GetMeshData()->NumSections; ++i)
		{
			Material* DrawMat = Mat != nullptr ? Mat : Mesh->GetMaterial(i);
			Mat->SetVec3("gLightPosition", glm::vec3(1000, 1700, 1000));
			Mat->SetMat4("gModel", Model);
			Mat->Use();

			glDrawElements(GL_TRIANGLES, Mesh->GetMeshData()->Sections[i].NumIndices, GL_UNSIGNED_INT, (GLvoid*)(sizeof(unsigned int) * Mesh->GetMeshData()->Sections[i].IndexOffset));
		
            BindMaterial(nullptr);
        }

		glBindVertexArray(0);
	}
}

void RenderManager::RenderScene(CameraComponent* Camera)
{
    RenderScene(nullptr, Camera);
}

void RenderManager::RenderScene(Shader* Shad, CameraComponent* Camera)
{
	BufferCameraData(Camera);
	BufferLightData();
	CameraBuffer->Bind(0);
	LightBuffer->Bind(1);


	for (size_t i = 0; i < Drawables.size(); ++i)
	{
		Drawables[i]->Draw(Shad);
	}
}

void RenderManager::RegisterDrawable(IDrawable* Drawable)
{
    Drawables.push_back(Drawable);
}

void RenderManager::DeregisterDrawable(IDrawable* Drawable)
{
    std::remove(Drawables.begin(), Drawables.end(), Drawable);
}

void RenderManager::SetMainCamera(CameraComponent* NewMainCamera)
{
    MainCamera = NewMainCamera;
}

void RenderManager::BufferCameraData(CameraComponent* Camera)
{
	CameraData.gEyePosition = Camera->GetPosition();
	CameraData.gProjectionView = Camera->GetProjectionView();
	CameraBuffer->BufferData(&CameraData, 0);
}

void RenderManager::BufferLightData()
{
    LightBuffer->ClearBufferData();
    LightData.NumPointLights = 0;

    for (int i = 0; i < Lights.size(); ++i)
    {
        LightComponent::Data* CurrLightData = Lights[i]->GetLightData();

        if (PointLightComponent::Data* PointLightData = static_cast<PointLightComponent::Data*>(CurrLightData))
        {
            memcpy(&LightData.PointLights[i], PointLightData, sizeof(PointLightComponent::Data));
            LightData.NumPointLights++;
        }
    }

    LightBuffer->BufferData(&LightData, 0);
}