#include "AssimpAssetFactory.h"

#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Material.h"
#include "StaticMesh.h"
#include "Texture.h"
#include "Core/Engine.h"
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

void AssimpAssetFactory::Init()
{
    AssetFactory::Init();

    AddSupportedFileExtension(".obj");
    AddSupportedFileExtension(".fbx");
}

void AssimpAssetFactory::Shutdown()
{
    AssetFactory::Shutdown();
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
    const aiScene* Scene = Importer.ReadFile(Source, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_PreTransformVertices | aiProcess_FlipUVs);

    if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << Importer.GetErrorString() << std::endl;
        ImportComplete(Package, false);
        return;
    }

    FProcessSceneParams Params;
    Params.Scene = Scene;
    Params.PackageHandle = Package;
    ProcessScene(Params);

    if(Params.CreateMeshParams.Positions.size() > 0)
    {
        StaticMesh* Mesh = StaticMesh::Create(this, Params.CreateMeshParams);
        GetAssetManager()->AddAsset(Mesh, Package);
    }
    
    ImportComplete(Package, true);
}

void AssimpAssetFactory::ProcessScene(FProcessSceneParams& Params)
{
    for (size_t i = 0; i < Params.Scene->mNumMaterials; ++i)
    {
        Material* Mat = ProcessMaterial(Params.Scene->mMaterials[i], Params);
        Engine::Get()->GetAssetManager()->AddAsset(Mat, Params.PackageHandle);
        Params.AllMaterials.push_back(Mat);
    }

    ProcessNode(Params.Scene->mRootNode, Params);
}

void AssimpAssetFactory::ProcessNode(aiNode* Node, FProcessSceneParams& Params)
{
    for (unsigned int i = 0; i < Node->mNumMeshes; i++)
    {
        aiMesh* AssimpMesh = Params.Scene->mMeshes[Node->mMeshes[i]];
        ProcessMesh(AssimpMesh, Node, Params);
    }

    for (unsigned int i = 0; i < Node->mNumChildren; i++)
    {
        ProcessNode(Node->mChildren[i], Params);
    }
}

void AssimpAssetFactory::ProcessMesh(aiMesh* Mesh, const aiNode* Node, FProcessSceneParams& Params)
{
    Material* Mat = Params.AllMaterials[Mesh->mMaterialIndex];

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

    StaticMesh::FCreateStaticMeshParams* MeshParams = &Params.CreateMeshParams;
    const unsigned int PositionsOffset = MeshParams->Positions.size();
    const unsigned int UVsOffset = MeshParams->UVs.size();
    const unsigned int NormalsOffset = MeshParams->Normals.size();
    const unsigned int TangentsOffset = MeshParams->Tangents.size();
    const unsigned int BitangentsOffset = MeshParams->Bitangents.size();
    const unsigned int ColorsOffset = MeshParams->Colors.size();

    MeshParams->Positions.resize(MeshParams->Positions.size() + (Mesh->HasPositions() ? Mesh->mNumVertices : 0));
    MeshParams->UVs.resize(MeshParams->UVs.size() + (Mesh->HasTextureCoords(0) ? Mesh->mNumVertices : 0));
    MeshParams->Normals.resize(MeshParams->Normals.size() + (Mesh->HasNormals() ? Mesh->mNumVertices : 0));
    MeshParams->Tangents.resize(MeshParams->Tangents.size() + (Mesh->HasTangentsAndBitangents() ? Mesh->mNumVertices : 0));
    MeshParams->Bitangents.resize(MeshParams->Bitangents.size() + (Mesh->HasTangentsAndBitangents() ? Mesh->mNumVertices : 0));
    MeshParams->Colors.resize(MeshParams->Colors.size() + (Mesh->HasVertexColors(0) ? Mesh->mNumVertices : 0));

    memcpy(MeshParams->Positions.data() + PositionsOffset, Mesh->mVertices, sizeof(glm::vec3) * (Mesh->HasPositions() ? Mesh->mNumVertices : 0));
    memcpy(MeshParams->Normals.data() + NormalsOffset, Mesh->mNormals, sizeof(glm::vec3) * (Mesh->HasNormals() ? Mesh->mNumVertices : 0));
    memcpy(MeshParams->Tangents.data() + TangentsOffset, Mesh->mTangents, sizeof(glm::vec3) * (Mesh->HasTangentsAndBitangents() ? Mesh->mNumVertices : 0));
    memcpy(MeshParams->Bitangents.data() + BitangentsOffset, Mesh->mBitangents, sizeof(glm::vec3) * (Mesh->HasTangentsAndBitangents() ? Mesh->mNumVertices : 0));
    memcpy(MeshParams->Colors.data() + ColorsOffset, Mesh->mColors[0], sizeof(glm::vec4) * (Mesh->HasVertexColors(0) ? Mesh->mNumVertices : 0));

    if(Mesh->HasTextureCoords(0))
    {
        for(unsigned i = 0; i < Mesh->mNumVertices; ++i)
        {
            MeshParams->UVs[i + UVsOffset] = glm::vec2(Mesh->mTextureCoords[0][i].x, Mesh->mTextureCoords[0][i].y);
        }
    }
    
    std::vector<unsigned int> Indices;
    for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
    {
        aiFace face = Mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            Indices.push_back(PositionsOffset + face.mIndices[j]);
        }
    }

    MeshParams->Triangles.push_back(Indices);
    MeshParams->Materials.push_back(Mat);
}

Material* AssimpAssetFactory::ProcessMaterial(aiMaterial* aiMat, FProcessSceneParams& Params)
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

    Texture* Tex = nullptr;
    if (DiffusePath.length > 0)
    {
        Tex = ImportTexture(DiffusePath.C_Str(), Params);
        Mat->SetTexture("gTex_Diffuse", Tex);
    }
    Mat->SetFloat("gTex_Diffuse_Power", Tex != nullptr ? 1.0f : 0.0f);

    aiString SpecularPath;
    aiMat->GetTexture(aiTextureType_SPECULAR, 0, &SpecularPath);

    Tex = nullptr;
    if (SpecularPath.length > 0)
    {
        Tex = ImportTexture(SpecularPath.C_Str(), Params);
        Mat->SetTexture("gTex_SpecularMask", Tex);
    }
    Mat->SetFloat("gTex_SpecularMask_Power", Tex != nullptr ? 1.0f : 0.0f);

    aiString NormalPath;
    aiMat->GetTexture(aiTextureType_NORMALS, 0, &NormalPath);

    Tex = nullptr;
    if (NormalPath.length > 0)
    {
        Tex = ImportTexture(NormalPath.C_Str(), Params);
        Mat->SetTexture("gTex_Normals", Tex);
    }
    Mat->SetFloat("gTex_Normals_Power", Tex != nullptr ? 1.0f : 0.0f);

    aiString BumpMapPath;
    aiMat->GetTexture(aiTextureType_HEIGHT, 0, &BumpMapPath);

    Tex = nullptr;
    if (BumpMapPath.length > 0)
    {
        Tex = ImportTexture(BumpMapPath.C_Str(), Params);
        Mat->SetTexture("gTex_Height", Tex);
    }
    Mat->SetFloat("gTex_Height_Power", Tex != nullptr ? 1.0f : 0.0f);

    Mat->SetFloat("gParallaxHeightScale", 0.1f);

    return Mat;
}

Texture* AssimpAssetFactory::ImportTexture(const std::string& Dir, FProcessSceneParams& Params)
{
    if(Params.LoadedTextures.find(Dir) != Params.LoadedTextures.end())
    {
        return Params.LoadedTextures[Dir];
    }
    
    int W; int H; int NumChannels;
    unsigned char* Pixels = nullptr;
    Texture::TextureFormats Format;

    if (const aiTexture* aiTex = Params.Scene->GetEmbeddedTexture(Dir.c_str()))
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
        AssetPackage* PackageObj = Engine::Get()->GetAssetManager()->GetPackageObject(Params.PackageHandle);
        std::string SourceDir = FileUtils::GetDirectory(PackageObj->GetSourcePath());

        std::string FinalPath;
        FileUtils::FindRelativePath(SourceDir, Dir, FinalPath);

        Pixels = SOIL_load_image(FinalPath.c_str(), &W, &H, &NumChannels, 0);
        Format = static_cast<Texture::TextureFormats>(Texture::TextureFormats::R8 + (NumChannels - 1));
    }

    if(Pixels == nullptr)
        return nullptr;

    Texture::CreateTextureParams CreateTexureParams;
    CreateTexureParams.W = W;
    CreateTexureParams.H = H;
    CreateTexureParams.Format = Format;
    CreateTexureParams.Pixels = Pixels;
    CreateTexureParams.GenerateMips = true;

    Texture* NewTex = Texture::Create(this, CreateTexureParams);
    Params.LoadedTextures[Dir] = NewTex;
    return NewTex;
}
