#include "StaticMesh.h"

#include <iostream>
#include <windows.h>
#include <glad/glad.h>

#include "Material.h"
#include "StaticMeshComponent.h"
#include "Core/Engine.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

StaticMesh* StaticMesh::Create(Object* NewOuter, const FCreateStaticMeshParams& Params)
{
    StaticMesh* NewMesh = Engine::Get()->CreateObject<StaticMesh>(NewOuter);
    MeshData* Data = NewMesh->GetMeshData();
    Data->SetPositions(Params.Positions.data(), Params.Positions.size());
    Data->SetUVs(Params.UVs.data(), Params.UVs.size());
    Data->SetNormals(Params.Normals.data(), Params.Normals.size());
    Data->SetTangents(Params.Tangents.data(), Params.Tangents.size());
    Data->SetBitangents(Params.Bitangents.data(), Params.Bitangents.size());
    Data->SetColors(Params.Colors.data(), Params.Colors.size());

    for(size_t i = 0; i < Params.Triangles.size(); ++i)
    {
        Data->AddSection(Params.Triangles[i].data(), Params.Triangles[i].size());
    }

    for(size_t i = 0; i < Params.Materials.size(); ++i)
    {
        NewMesh->SetMaterial(i, Params.Materials[i]);
    }
    
    NewMesh->CommitMeshData();

    return NewMesh;
}

StaticMesh::MeshData::MeshData()
{
    NumSections = 0;
    Sections = new MeshSection[MAX_MESH_SECTIONS];

    NumIndices = 0;
    Indices = nullptr;

    Positions = nullptr;
    UVs = nullptr;
    Normals = nullptr;
    Tangents = nullptr;
    Bitangents = nullptr;
    Colors = nullptr;

    NumPositions = 0;
    NumUVs = 0;
    NumNormals = 0;
    NumTangents = 0;
    NumBitangents = 0;
    NumColors = 0;

    glGenBuffers(1, &IndexBuffer);
    glGenBuffers(1, &PositionBuffer);
    glGenBuffers(1, &UVsBuffer);
    glGenBuffers(1, &NormalsBuffer);
    glGenBuffers(1, &TangentsBuffer);
    glGenBuffers(1, &BitangentsBuffer);
    glGenBuffers(1, &ColorsBuffer);
    glGenVertexArrays(1, &VertexArrayObject);
}

StaticMesh::MeshData::~MeshData()
{
    for (int i = NumSections - 1; i >= 0; --i)
    {
        RemoveSection(i);
    }
    delete[] Sections;

    glDeleteBuffers(1, &IndexBuffer);
    glDeleteBuffers(1, &PositionBuffer);
    glDeleteBuffers(1, &UVsBuffer);
    glDeleteBuffers(1, &NormalsBuffer);
    glDeleteBuffers(1, &TangentsBuffer);
    glDeleteBuffers(1, &BitangentsBuffer);
    glDeleteBuffers(1, &ColorsBuffer);
    glDeleteVertexArrays(1, &VertexArrayObject);
}

void StaticMesh::MeshData::SetPositions(const glm::vec3* InPositions, unsigned int Size)
{
    delete[] Positions;
    Positions = new glm::vec3[Size];
    memcpy(Positions, InPositions, sizeof(glm::vec3) * Size);
    NumPositions = Size;
}

void StaticMesh::MeshData::SetUVs(const glm::vec3* InUVs, unsigned int Size)
{
    delete[] UVs;
    UVs = new glm::vec3[Size];
    memcpy(UVs, InUVs, sizeof(glm::vec3) * Size);
    NumUVs = Size;
}

void StaticMesh::MeshData::SetNormals(const glm::vec3* InNormals, unsigned int Size)
{
    delete[] Normals;
    Normals = new glm::vec3[Size];
    memcpy(Normals, InNormals, sizeof(glm::vec3) * Size);
    NumNormals = Size;
}

void StaticMesh::MeshData::SetTangents(const glm::vec3* InTangents, unsigned int Size)
{
    delete[] Tangents;
    Tangents = new glm::vec3[Size];
    memcpy(Tangents, InTangents, sizeof(glm::vec3) * Size);
    NumTangents = Size;
}

void StaticMesh::MeshData::SetBitangents(const glm::vec3* InBitangents, unsigned int Size)
{
    delete[] Bitangents;
    Bitangents = new glm::vec3[Size];
    memcpy(Bitangents, InBitangents, sizeof(glm::vec3) * Size);
    NumBitangents = Size;
}

void StaticMesh::MeshData::SetColors(const glm::vec4* InColors, unsigned int Size)
{
    delete[] Colors;
    Colors = new glm::vec4[Size];
    memcpy(Colors, InColors, sizeof(glm::vec4) * Size);
    NumColors = Size;
}

void StaticMesh::MeshData::CommitData()
{
    glBindVertexArray(VertexArrayObject);
    
    glBindBuffer(GL_ARRAY_BUFFER, PositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * NumPositions, Positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, UVsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * NumUVs, UVs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, NormalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * NumNormals, Normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, TangentsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * NumTangents, Tangents, GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, BitangentsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * NumBitangents, Bitangents, GL_STATIC_DRAW);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, ColorsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * NumColors, Colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumIndices * sizeof(unsigned int), Indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void StaticMesh::MeshData::AddSection(const unsigned int* SectionIndices, unsigned int Size)
{
    if (NumSections >= MAX_MESH_SECTIONS)
        return;

    unsigned int* NewIndices = new unsigned int[NumIndices + Size];
    memcpy(NewIndices, Indices, sizeof(unsigned int) * NumIndices);
    memcpy((NewIndices + NumIndices), SectionIndices, sizeof(unsigned int) * Size);
    delete[] Indices;
    Indices = NewIndices;
    
    MeshSection* NewMeshSection = &Sections[NumSections++];
    NewMeshSection->IndexOffset = NumIndices;
    NewMeshSection->NumIndices = Size;
    NumIndices += Size;
}

void StaticMesh::MeshData::RemoveSection(unsigned int SectionIndex)
{
    if (SectionIndex >= NumSections)
        return;
    
    MeshSection* Section = &Sections[SectionIndex];
    unsigned int Start = Section->IndexOffset;
    unsigned int End = Section->IndexOffset + Section->NumIndices;
    unsigned int Num = Section->NumIndices;
    unsigned int NewNum = NumIndices - Num;
    
    unsigned int* NewIndices = new unsigned int[NewNum];
    memcpy(NewIndices, Indices, sizeof(unsigned int) * Start);
    memcpy((NewIndices + Start), (Indices + End), sizeof(unsigned int) * (NumIndices - End));
    NumIndices = NewNum;
    
    delete[] Indices;
    Indices = NewIndices;

    for(unsigned int i = SectionIndex; i < NumSections - 1; ++i)
    {
        Sections[i] = Sections[i + 1];
        Sections[i].IndexOffset -= Num;
    }
    NumSections--;
}

unsigned StaticMesh::MeshData::GetNumSections() const
{
    return NumSections;
}

bool StaticMesh::MeshData::GetMeshSection(unsigned SectionIndex, MeshSection& Section) const
{
    if(SectionIndex >= NumSections)
        return false;

    Section = Sections[SectionIndex];
    return true;
}

StaticMesh::StaticMesh(Object* NewOuter) : AssetResource(NewOuter)
{
    Data = new MeshData();
}

StaticMesh::~StaticMesh()
{
    delete Data;
}

Component* StaticMesh::SpawnComponentForAsset()
{
    return CreateObject<StaticMeshComponent>(this);
}

void StaticMesh::ApplyToComponent(Component* Component)
{
    if(StaticMeshComponent* SMComp = dynamic_cast<StaticMeshComponent*>(Component))
    {
        SMComp->SetMesh(this);
    }
}

void StaticMesh::SetMaterial(unsigned Index, Material* NewMat)
{
    Materials.resize(std::max(Index + 1, (unsigned int)Materials.size()));
    Materials[Index] = NewMat;
}

Material* StaticMesh::GetMaterial(unsigned Index) const
{
    return Materials[Index];
}

unsigned StaticMesh::GetNumMaterials() const
{
    return Materials.size();
}

StaticMesh::MeshData* StaticMesh::GetMeshData() const
{
    return Data;
}

void StaticMesh::CommitMeshData()
{
    Data->CommitData();
}

void StaticMesh::Draw(const glm::vec3& ViewPos, const glm::mat4& View, const glm::mat4& Projection, const glm::mat4& Model)
{    
    glBindVertexArray(Data->VertexArrayObject);

    for(unsigned int i = 0; i < Data->NumSections; ++i)
    {
        Material* Mat = GetMaterial(i);
        Mat->Use();
        Mat->SetMat4("gViewProjection", Projection * View);
        Mat->SetVec3("gEyePosition", ViewPos); 
        Mat->SetVec3("gLightPosition", glm::vec3(1000, 1700, 1000));
        Mat->SetMat4("gModel", Model);
        
        glDrawElements(GL_TRIANGLES, Data->Sections[i].NumIndices, GL_UNSIGNED_INT, (GLvoid*)(sizeof(unsigned int) * Data->Sections[i].IndexOffset));
    }

    glBindVertexArray(0);
}
