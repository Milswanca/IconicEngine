#include "AssimpAssetFactory.h"

#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Material.h"
#include "StaticMesh.h"
#include "Texture.h"
#include "Core/Engine.h"
#include "lodepng/lodepng.h"
#include <iostream>
#include "SOIL/SOIL.h"
#include "Utils/FileUtils.h"

std::map<std::string, Texture::TextureFormats> AssimpAssetFactory::AssimpToTextureFormatEnum =
{
    {"rgba8888", Texture::TextureFormats::RGBA8},
    {"rgba8880", Texture::TextureFormats::RGB8},
    {"rgba8800", Texture::TextureFormats::RG8},
    {"rgba8000", Texture::TextureFormats::R8},
};

AssimpAssetFactory::AssimpAssetFactory(Object* NewOuter) : AssetFactory(NewOuter)
{
    AddSupportedFileExtension(".obj");
    AddSupportedFileExtension(".fbx");
}

AssimpAssetFactory::~AssimpAssetFactory()
{
}

void AssimpAssetFactory::ImportPackage(PACKAGE_HANDLE Package)
{
    AssetPackage* PackageObject = Engine::Get()->GetAssetManager()->GetPackageObject(Package);

    if (PackageObject == nullptr)
    {
        ImportComplete(Package, false);
        return;
    }

    std::string Source = PackageObject->GetSourcePath();
    
    Assimp::Importer Importer;
    const aiScene* Scene = Importer.ReadFile(Source, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs);

    if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << Importer.GetErrorString() << std::endl;
        ImportComplete(Package, false);
        return;
    }

    ProcessScene(Scene, Package);
    ImportComplete(Package, true);
}

void AssimpAssetFactory::ProcessScene(const aiScene* Scene, PACKAGE_HANDLE Package)
{
    std::vector<Material*> AllMaterials;
    for (size_t i = 0; i < Scene->mNumMaterials; ++i)
    {
        Material* Mat = ProcessMaterial(Scene->mMaterials[i], Scene, Package);
        Engine::Get()->GetAssetManager()->AddAsset(Mat, Package);
        AllMaterials.push_back(Mat);
    }

    ProcessNode(Scene->mRootNode, AllMaterials, Scene, Package);
}

void AssimpAssetFactory::ProcessNode(aiNode* Node, const std::vector<Material*>& AllMaterials, const aiScene* Scene, PACKAGE_HANDLE Package)
{
    for (unsigned int i = 0; i < Node->mNumMeshes; i++)
    {
        aiMesh* AssimpMesh = Scene->mMeshes[Node->mMeshes[i]];
        StaticMesh* Mesh = ProcessMesh(AssimpMesh, Node, AllMaterials, Scene, Package);
        Engine::Get()->GetAssetManager()->AddAsset(Mesh, Package);
    }

    for (unsigned int i = 0; i < Node->mNumChildren; i++)
    {
        ProcessNode(Node->mChildren[i], AllMaterials, Scene, Package);
    }
}

StaticMesh* AssimpAssetFactory::ProcessMesh(aiMesh* Mesh, const aiNode* Node, const std::vector<Material*>& AllMaterials, const aiScene* Scene, PACKAGE_HANDLE Package)
{
    Material* Mat = AllMaterials[Mesh->mMaterialIndex];

    aiMatrix4x4 Transform = Node->mTransformation;
    const aiNode* CurrentNode = Node;
    while (CurrentNode->mParent != nullptr)
    {
        Transform = CurrentNode->mParent->mTransformation * Transform;
        CurrentNode = CurrentNode->mParent;
    }

    aiMatrix4x4 InversedTrans = Transform;
    aiVector3D Position;
    aiQuaternion Rotation;
    InversedTrans.Inverse().DecomposeNoScaling(Rotation, Position);

    for (size_t i = 0; i < Mesh->mNumVertices; ++i)
    {
        if (Mesh->HasPositions())
            Mesh->mVertices[i] = Transform * Mesh->mVertices[i];

        if (Mesh->HasNormals())
            Mesh->mNormals[i] = (aiMatrix3x3(Transform) * Mesh->mNormals[i]).Normalize();
    }

    StaticMesh::FCreateStaticMeshParams Params;
    Params.Positions = std::vector<glm::vec3>(Mesh->HasPositions() ? Mesh->mNumVertices : 0);
    Params.UVs = std::vector<glm::vec3>(Mesh->HasTextureCoords(0) ? Mesh->mNumVertices : 0);
    Params.Normals = std::vector<glm::vec3>(Mesh->HasNormals() ? Mesh->mNumVertices : 0);
    Params.Tangents = std::vector<glm::vec3>(Mesh->HasTangentsAndBitangents() ? Mesh->mNumVertices : 0);
    Params.Bitangents = std::vector<glm::vec3>(Mesh->HasTangentsAndBitangents() ? Mesh->mNumVertices : 0);
    Params.Colors = std::vector<glm::vec4>(Mesh->HasVertexColors(0) ? Mesh->mNumVertices : 0);

    memcpy(Params.Positions.data(), Mesh->mVertices, sizeof(glm::vec3) * (Mesh->HasPositions() ? Mesh->mNumVertices : 0));
    memcpy(Params.UVs.data(), Mesh->mTextureCoords[0], sizeof(glm::vec3) * (Mesh->HasTextureCoords(0) ? Mesh->mNumVertices : 0));
    memcpy(Params.Normals.data(), Mesh->mNormals, sizeof(glm::vec3) * (Mesh->HasNormals() ? Mesh->mNumVertices : 0));
    memcpy(Params.Tangents.data(), Mesh->mTangents, sizeof(glm::vec3) * (Mesh->HasTangentsAndBitangents() ? Mesh->mNumVertices : 0));
    memcpy(Params.Bitangents.data(), Mesh->mBitangents, sizeof(glm::vec3) * (Mesh->HasTangentsAndBitangents() ? Mesh->mNumVertices : 0));
    memcpy(Params.Colors.data(), Mesh->mColors, sizeof(glm::vec4) * (Mesh->HasVertexColors(0) ? Mesh->mNumVertices : 0));

    std::vector<unsigned int> Indices;
    for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
    {
        aiFace face = Mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            Indices.push_back(face.mIndices[j]);
        }
    }
    
    Params.Triangles.push_back(Indices);
    Params.Materials.push_back(Mat);

    return StaticMesh::Create(this, Params);
}

Material* AssimpAssetFactory::ProcessMaterial(aiMaterial* aiMat, const aiScene* Scene, PACKAGE_HANDLE Package)
{
    Material* Mat = CreateObject<Material>(this);
    Mat->SetShader(Engine::BaseShader);

    aiString MatName = aiMat->GetName();
    Mat->SetName(MatName.C_Str());
    
    aiColor3D DiffuseColor;
    aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor);
    Mat->SetVec3("gDiffuseColor", glm::vec3(DiffuseColor.r, DiffuseColor.g, DiffuseColor.b));

    aiColor3D AmbientColor;
    aiMat->Get(AI_MATKEY_COLOR_AMBIENT, AmbientColor);
    Mat->SetVec3("gAmbientColor", glm::vec3(AmbientColor.r, AmbientColor.g, AmbientColor.b));

    float Shininess = 1.0f;
    aiMat->Get(AI_MATKEY_SHININESS, Shininess);
    Mat->SetFloat("gShininess", Shininess);

    float ShininessStrength = 1.0f;
    aiMat->Get(AI_MATKEY_SHININESS_STRENGTH, ShininessStrength);
    Mat->SetFloat("gShininessStrength", ShininessStrength);

    aiString DiffusePath;
    aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &DiffusePath);

    if (DiffusePath.length > 0)
    {
        Texture* Tex = ImportTexture(DiffusePath.C_Str(), Scene, Package);
        Mat->SetFloat("gTex_Diffuse_Power", Tex != nullptr ? 1.0f : 0.0f);
        Mat->SetTexture("gTex_Diffuse", Tex);
    }

    aiString SpecularPath;
    aiMat->GetTexture(aiTextureType_SPECULAR, 0, &SpecularPath);
    
    if (SpecularPath.length > 0)
    {
        Texture* Tex = ImportTexture(SpecularPath.C_Str(), Scene, Package);
        Mat->SetFloat("gTex_SpecularMask_Power", Tex != nullptr ? 1.0f : 0.0f);
        Mat->SetTexture("gTex_SpecularMask", Tex);
    }

    aiString NormalPath;
    aiMat->GetTexture(aiTextureType_NORMALS, 0, &NormalPath);

    if (NormalPath.length > 0)
    {
        Texture* Tex = ImportTexture(NormalPath.C_Str(), Scene, Package);
        Mat->SetFloat("gTex_Normals_Power", Tex != nullptr ? 1.0f : 0.0f);
        Mat->SetTexture("gTex_Normals", Tex);
    }

    aiString BumpMapPath;
    aiMat->GetTexture(aiTextureType_HEIGHT, 0, &BumpMapPath);

    if (BumpMapPath.length > 0)
    {
        Texture* Tex = ImportTexture(BumpMapPath.C_Str(), Scene, Package);
        Mat->SetFloat("gTex_Height_Power", Tex != nullptr ? 1.0f : 0.0f);
        Mat->SetTexture("gTex_Height", Tex);
    }

    Mat->SetFloat("gParallaxHeightScale", 0.1f);

    return Mat;
}

Texture* AssimpAssetFactory::ImportTexture(const std::string& Dir, const aiScene* Scene, PACKAGE_HANDLE Package)
{
    int W; int H; int NumChannels;
    unsigned char* Pixels = nullptr;
    Texture::TextureFormats Format;

    if (const aiTexture* aiTex = Scene->GetEmbeddedTexture(Dir.c_str()))
    {
        Pixels = reinterpret_cast<unsigned char*>(&aiTex->pcData[0]);
        Format = AssimpToTextureFormatEnum[aiTex->achFormatHint];

        if(strcmp(aiTex->achFormatHint, "png") == 0 || strcmp(aiTex->achFormatHint, "jpg") == 0)
        {
            Pixels = SOIL_load_image_from_memory(Pixels, aiTex->mWidth, &W, &H, &NumChannels, 0);
            Format = static_cast<Texture::TextureFormats>(Texture::TextureFormats::R8 + (NumChannels - 1));
        }
    }
    else
    {
        AssetPackage* PackageObj = Engine::Get()->GetAssetManager()->GetPackageObject(Package);
        std::string SourceDir = FileUtils::GetDirectory(PackageObj->GetSourcePath());

        std::string FinalPath;
        FileUtils::FindRelativePath(SourceDir, Dir, FinalPath);

        Pixels = SOIL_load_image(FinalPath.c_str(), &W, &H, &NumChannels, 0);
        Format = static_cast<Texture::TextureFormats>(Texture::TextureFormats::R8 + (NumChannels - 1));
    }

    if(Pixels == nullptr)
        return nullptr;

    Texture* Tex = CreateObject<Texture>(this);
    Tex->Initialize(W, H, Format);
    Tex->SetPixels(Pixels);
    Tex->UpdateResource(true);

    return Tex;
}
