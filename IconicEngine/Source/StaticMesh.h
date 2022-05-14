#pragma once
#include <glm/fwd.hpp>

#include "Assets/AssetManager.h"
#include "Core/Object.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

class Material;
class Object;

class StaticMesh : public AssetResource
{
public:
    struct FCreateStaticMeshParams
    {
        std::vector<glm::vec3> Positions;
        std::vector<glm::vec3> UVs;
        std::vector<glm::vec3> Normals;
        std::vector<glm::vec3> Tangents;
        std::vector<glm::vec3> Bitangents;
        std::vector<glm::vec4> Colors;
        std::vector<std::vector<unsigned int>> Triangles;
        std::vector<Material*> Materials;
    };
    
    static const int MAX_MESH_SECTIONS = 8;

    struct MeshSection
    {
        unsigned int IndexOffset;
        unsigned int NumIndices;
    };
    
    struct MeshData
    {
        MeshData();
        ~MeshData();

    public:
        void SetPositions(const glm::vec3* InPositions, unsigned int Size);
        void SetUVs(const glm::vec3* InUVs, unsigned int Size);
        void SetNormals(const glm::vec3* InNormals, unsigned int Size);
        void SetTangents(const glm::vec3* InTangents, unsigned int Size);
        void SetBitangents(const glm::vec3* InBitangents, unsigned int Size);
        void SetColors(const glm::vec4* InColors, unsigned int Size);
        
        void AddSection(const unsigned int* SectionIndices, unsigned int Size);
        void RemoveSection(unsigned int Section);
        unsigned int GetNumSections() const;
        bool GetMeshSection(unsigned int SectionIndex, MeshSection& Section) const;

    private:
        unsigned int PositionBuffer;
        unsigned int UVsBuffer;
        unsigned int NormalsBuffer;
        unsigned int TangentsBuffer;
        unsigned int BitangentsBuffer;
        unsigned int ColorsBuffer;
        unsigned int IndexBuffer;
        unsigned int VertexArrayObject;
        
        unsigned int* Indices;
        unsigned int NumIndices;
        
        glm::vec3* Positions;
        glm::vec3* UVs;
        glm::vec3* Normals;
        glm::vec3* Tangents;
        glm::vec3* Bitangents;
        glm::vec4* Colors;
        MeshSection* Sections;

        unsigned int NumPositions;
        unsigned int NumUVs;
        unsigned int NumNormals;
        unsigned int NumTangents;
        unsigned int NumBitangents;
        unsigned int NumColors;
        unsigned int NumSections;

        void CommitData();
        
        friend class StaticMesh;
    };

public:
    static StaticMesh* Create(Object* NewOuter, const FCreateStaticMeshParams& Params);
    
    StaticMesh(Object* NewOuter);
    virtual ~StaticMesh();
    
    virtual Component* SpawnComponentForAsset() override;
    virtual void ApplyToComponent(Component* Component) override;
    
    void SetMaterial(unsigned int Index, Material* NewMat);
    Material* GetMaterial(unsigned int Index) const;
    unsigned int GetNumMaterials() const;
    
    MeshData* GetMeshData() const;
    void CommitMeshData();

    void Draw(const glm::vec3& ViewPos, const glm::mat4& View, const glm::mat4& Projection, const glm::mat4& Model);
    
private:
    std::vector<Material*> Materials;
    MeshData* Data;
};
