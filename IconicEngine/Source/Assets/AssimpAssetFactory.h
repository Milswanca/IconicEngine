#pragma once
#include <map>
#include <vector>
#include <assimp/scene.h>

#include "AssetFactory.h"
#include "Texture.h"

class Texture;
class Material;
class StaticMesh;

class AssimpAssetFactory : public AssetFactory
{    
public:
    AssimpAssetFactory(Object* NewOuter);
    virtual ~AssimpAssetFactory();

protected:
    virtual void ImportPackage(PACKAGE_HANDLE Package) override;
    
    void ProcessScene(const aiScene* Scene, PACKAGE_HANDLE Package);
    void ProcessNode(aiNode* Node, const std::vector<Material*>& AllMaterials, const aiScene* Scene, PACKAGE_HANDLE Package);
    
    StaticMesh* ProcessMesh(aiMesh* Mesh, const aiNode* Node, const std::vector<Material*>& AllMaterials, const aiScene* Scene, PACKAGE_HANDLE Package);
    Material* ProcessMaterial(aiMaterial* aiMat, const aiScene* Scene, PACKAGE_HANDLE Package);
    Texture* ImportTexture(const std::string& Dir, const aiScene* Scene, PACKAGE_HANDLE Package);

public:
    static std::map<std::string, Texture::TextureFormats> AssimpToTextureFormatEnum;
};
