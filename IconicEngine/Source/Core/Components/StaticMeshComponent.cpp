#include "StaticMeshComponent.h"

#include "Rendering/Material.h"
#include "Rendering/RenderManager.h"
#include "Rendering/Shader.h"

void StaticMeshComponent::Init()
{
    Component::Init();

    GetRenderManager()->RegisterDrawable(this);
}

void StaticMeshComponent::Shutdown()
{
    Component::Shutdown();

    GetRenderManager()->DeregisterDrawable(this);
}

void StaticMeshComponent::Draw(Shader* Shad)
{
    glm::mat4 LocalToWorld;
    GetLocalToWorld(LocalToWorld);
    
    if(Mesh)
    {
		glBindVertexArray(Mesh->GetMeshData()->VertexArrayObject);

		for (unsigned int i = 0; i < Mesh->GetMeshData()->NumSections; ++i)
		{
			Material* DrawMat = Mesh->GetMaterial(i);

			Shader* OldShader = DrawMat->GetShader();
			if(Shad)
			{
				DrawMat->SetShader(Shad);
			}

			DrawMat->SetVec3("gLightPosition", glm::vec3(1000, 1700, 1000));
			DrawMat->SetMat4("gModel", LocalToWorld);			
			DrawMat->Use();
			DrawMat->BindParameters();

			glDrawElements(GL_TRIANGLES, Mesh->GetMeshData()->Sections[i].NumIndices, GL_UNSIGNED_INT, (GLvoid*)(sizeof(unsigned int) * Mesh->GetMeshData()->Sections[i].IndexOffset));

			DrawMat->SetShader(OldShader);
		}

		glBindVertexArray(0);
    }
}

void StaticMeshComponent::SetMesh(StaticMesh* NewMesh)
{
    Mesh = NewMesh;
}

StaticMesh* StaticMeshComponent::GetMesh() const
{
    return Mesh;
}