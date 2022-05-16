#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include <assimp/scene.h>

#include "AssetFactory.h"
#include "StaticMesh.h"
#include "Texture.h"

class Texture;
class Material;
class StaticMesh;

class AssimpAssetFactory : public AssetFactory
{
protected:
    struct FProcessSceneParams
    {
        std::vector<Material*> AllMaterials;
        const aiScene* Scene;
        PACKAGE_HANDLE PackageHandle;
        StaticMesh::FCreateStaticMeshParams CreateMeshParams;
        std::unordered_map<std::string, Texture*> LoadedTextures;
    };
    
public:
    IMPLEMENT_CONSTRUCTOR(AssimpAssetFactory, AssetFactory);

    virtual void Init() override;
    virtual void Shutdown() override;

protected:
    virtual void ImportPackage(PACKAGE_HANDLE Package) override;
    
    void ProcessScene(FProcessSceneParams& Params);
    void ProcessNode(aiNode* Node, FProcessSceneParams& Params);
    
    void ProcessMesh(aiMesh* Mesh, const aiNode* Node, FProcessSceneParams& Params);
    Material* ProcessMaterial(aiMaterial* aiMat, FProcessSceneParams& Params);
    Texture* ImportTexture(const std::string& Dir, FProcessSceneParams& Params);

public:
    static std::map<std::string, Texture::TextureFormats> AssimpToTextureFormatEnum;
};
