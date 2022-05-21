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
			Material* DrawMat = GetMaterial(i);

			Shader* OldShader = DrawMat->GetShader();
			if(Shad)
			{
				DrawMat->SetShader(Shad);
			}

			DrawMat->SetVec3("gLightPosition", glm::vec3(1000, 1700, 1000));
			DrawMat->SetMat4("gModel", LocalToWorld);			
			DrawMat->Use();

			glDrawElements(GL_TRIANGLES, Mesh->GetMeshData()->Sections[i].NumIndices, GL_UNSIGNED_INT, (GLvoid*)(sizeof(unsigned int) * Mesh->GetMeshData()->Sections[i].IndexOffset));

			GetRenderManager()->BindMaterial(nullptr);
			DrawMat->SetShader(OldShader);
		}

		glBindVertexArray(0);
    }
}

void StaticMeshComponent::SetMesh(StaticMesh* NewMesh)
{
    Mesh = NewMesh;

	if (NewMesh != nullptr)
	{
		for (unsigned int i = 0; i < NewMesh->GetNumMaterials(); ++i)
		{
			SetMaterial(i, NewMesh->GetMaterial(i));
		}
	}
}

StaticMesh* StaticMeshComponent::GetMesh() const
{
    return Mesh;
}

void StaticMeshComponent::SetMaterial(unsigned int Index, Material* Material)
{
	Materials.resize(std::max(Index + 1, (unsigned int)Materials.size()));
	Materials[Index] = Material;
}

Material* StaticMeshComponent::GetMaterial(unsigned int Index) const
{
	return Materials[Index];
}

unsigned int StaticMeshComponent::GetNumMaterials() const
{
	return Materials.size();
}