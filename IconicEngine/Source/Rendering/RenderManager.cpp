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
    
    CameraBuffer = CreateObject<UniformBufferObject>(this);
    DeferredBuffer = CreateObject<GBufferDeferred>(this);
}

void RenderManager::Shutdown()
{
    Object::Shutdown();
}

void RenderManager::BindMaterial(Material* Mat)
{
    if(BoundMat == Mat)
        return;

    if(BoundMat)
        BoundMat->MaterialUnbound();

    if(Mat->GetProgramID() != BoundProgramID)
        glUseProgram(Mat != nullptr ? Mat->GetProgramID() : 0);

    BoundProgramID = Mat->GetProgramID();
    BoundMat = Mat;

    if(BoundMat)
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
        GBuffer::GBufferPass* Pass = DeferredBuffer->GetPassData(i);

        if(Pass)
        {
            DeferredBuffer->PreRenderPass(i);

            switch(Pass->RenderType)
            {
            case GBuffer::GBufferRenderPassType::RenderScene:
                RenderScene(Pass->RenderShader, MainCamera);
                break;

            case GBuffer::GBufferRenderPassType::RenderQuad:
                RenderMesh(MainCamera, glm::identity<glm::mat4>(), Pass->RenderMaterial, QuadMesh);
                break;
            }

            DeferredBuffer->PostRenderPass(i);
        }
    }
}

void RenderManager::RenderMesh(CameraComponent* Camera, const glm::mat4& Model, Material* Mat, StaticMesh* Mesh)
{
	CameraData.gEyePosition = Camera->GetPosition();
	CameraData.gProjectionView = Camera->GetProjectionView();
	CameraBuffer->BufferData(&CameraData);
	CameraBuffer->Bind(0);

	if (Mesh)
	{
		glBindVertexArray(Mesh->GetMeshData()->VertexArrayObject);

		for (unsigned int i = 0; i < Mesh->GetMeshData()->NumSections; ++i)
		{
			Material* DrawMat = Mat != nullptr ? Mat : Mesh->GetMaterial(i);
			Mat->SetVec3("gLightPosition", glm::vec3(1000, 1700, 1000));
			Mat->SetMat4("gModel", Model);
			Mat->Use();
			Mat->BindParameters();

			glDrawElements(GL_TRIANGLES, Mesh->GetMeshData()->Sections[i].NumIndices, GL_UNSIGNED_INT, (GLvoid*)(sizeof(unsigned int) * Mesh->GetMeshData()->Sections[i].IndexOffset));
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
	CameraData.gEyePosition = Camera->GetPosition();
	CameraData.gProjectionView = Camera->GetProjectionView();
	CameraBuffer->BufferData(&CameraData);
	CameraBuffer->Bind(0);

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
