#pragma once
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "AssetTypes.h"
#include "Component.h"
#include "IndexArray.h"
#include "Core/Object.h"
#include "Utils/StringUtils.h"

class AssetFactory;

class AssetResource : public Object
{
public:
    IMPLEMENT_CONSTRUCTOR(AssetResource, Object);

    virtual Component* SpawnComponentForAsset();
    virtual void ApplyToComponent(Component* Component);
    
private:
    ASSET_HANDLE Handle;
    PACKAGE_HANDLE OwningPackage;
    std::string AssetName;
    
    friend class AssetManager;
};

class AssetPackage : public Object
{
public:
    AssetPackage(Object* NewOuter);
    virtual ~AssetPackage();
    
    std::string GetSourcePath() const;
    
private:
    PACKAGE_HANDLE Handle;
    std::vector<ASSET_HANDLE> Assets;
    std::string PackageName;
    std::string Source;
    
    friend class AssetManager;
};

class AssetManager : public Object
{
public:
    AssetManager(Object* NewOuter);
    ~AssetManager();

    PACKAGE_HANDLE QueueImport(const std::string& File);

    PACKAGE_HANDLE CreatePackage(const std::string& PackageName, const std::string& Source);
    void DeletePackage(PACKAGE_HANDLE Package);
    
    ASSET_HANDLE AddAsset(AssetResource* Asset, PACKAGE_HANDLE Package);
    void RemoveAsset(ASSET_HANDLE Asset);

    AssetResource* GetAssetObject(ASSET_HANDLE Handle) const;
    AssetPackage* GetPackageObject(PACKAGE_HANDLE Handle) const;

    bool IsValidAsset(ASSET_HANDLE Handle) const;
    bool IsValidPackage(PACKAGE_HANDLE Handle) const;

    template<typename T>
    T* GetFirstAssetOfTypeInPackage(PACKAGE_HANDLE Handle) const
    {
        AssetPackage* Package = GetPackageObject(Handle);

        if(Package)
        {
            for(size_t i = 0; i < Package->Assets.size(); ++i)
            {
                if(T* Casted = static_cast<T*>(GetAssetObject(Package->Assets[i])))
                {
                    return Casted;
                }
            }
        }

        return nullptr;
    }

    template<typename T>
    void GetAllAssetsOfTypeInPackage(PACKAGE_HANDLE Handle, std::vector<T*>& OutAssets) const
    {
        AssetPackage* Package = GetPackageObject(Handle);

        if(Package)
        {
            for(size_t i = 0; i < Package->Assets.size(); ++i)
            {
                if(T* Casted = dynamic_cast<T*>(GetAssetObject(Package->Assets[i])))
                {
                    OutAssets.push_back(Casted);
                }
            }
        }
    }

public:
    template<typename T>
    void RegisterFactory()
    {
        T* Factory = CreateObject<T>(this);

        std::string Extensions;
        Factory->GetSupportedFileExtensions(Extensions);

        std::vector<std::string> Split = StringUtils::Split(Extensions);

        for(size_t i = 0; i < Split.size(); ++i)
        {
            if(Factories.find(Split[i]) == Factories.end())
            {
                Factories[Split[i]] = Factory;
            }
        }
    }

    template<typename T>
    void DeregisterFactory()
    {
        for (auto Iter = Factories.begin(); Iter != Factories.end();)
        {
            if (T* Factory = static_cast<T*>(Iter->second))
            {
                Factories.erase(Iter++);
            }
            else
            {
                ++Iter;
            }
        }
    }

    AssetFactory* GetAssetFactory(const std::string& Extension) const;

private:
    void ImportNextPackage();
    void ImportComplete(PACKAGE_HANDLE Package, bool bSuccess);
    
private:
    AssetResource** Assets;
    AssetPackage** Packages;
    IndexArray<unsigned int>* AssetIDs;
    IndexArray<unsigned int>* PackageIDs;
    std::map<std::string, AssetFactory*> Factories;
    std::queue<PACKAGE_HANDLE> ImportQueue;
    bool bIsImporting;

    friend class AssetFactory;
};
