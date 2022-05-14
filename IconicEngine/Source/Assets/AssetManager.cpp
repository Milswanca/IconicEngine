#include "AssetManager.h"

#include "AssimpAssetFactory.h"
#include "Core/Engine.h"
#include "Utils/FileUtils.h"

AssetResource::AssetResource(Object* NewOuter) : Object(NewOuter)
{
}

AssetResource::~AssetResource()
{
}

Component* AssetResource::SpawnComponentForAsset()
{
    return nullptr;
}

void AssetResource::ApplyToComponent(Component* Component)
{
}

AssetPackage::AssetPackage(Object* NewOuter) : Object(NewOuter)
{
}

AssetPackage::~AssetPackage()
{
}

std::string AssetPackage::GetSourcePath() const
{
    return Source;
}

AssetManager::AssetManager(Object* NewOuter) : Object(NewOuter)
{
    Assets = new AssetResource*[MAX_ASSETS];
    Packages = new AssetPackage*[MAX_ASSETS];
    AssetIDs = new IndexArray<unsigned int>(MAX_ASSETS);
    PackageIDs = new IndexArray<unsigned int>(MAX_ASSETS);
    bIsImporting = false;

    RegisterFactory<AssimpAssetFactory>();
}

AssetManager::~AssetManager()
{
    delete[] Assets;
    delete[] Packages;
    delete AssetIDs;
    delete PackageIDs;
}

PACKAGE_HANDLE AssetManager::QueueImport(const std::string& File)
{
    PACKAGE_HANDLE Handle = CreatePackage(File, File);
    ImportQueue.push(Handle);

    if (!bIsImporting)
    {
        ImportNextPackage();
    }

    return Handle;
}

void AssetManager::ImportNextPackage()
{
    if (ImportQueue.empty())
        return;

    PACKAGE_HANDLE Next = ImportQueue.front();
    ImportQueue.pop();

    AssetPackage* PackageObject = GetPackageObject(Next);

    if (PackageObject)
    {
        std::string Extension = FileUtils::GetPathExtension(PackageObject->Source, true);
        AssetFactory* Factory = GetAssetFactory(Extension);

        if (Factory)
        {
            bIsImporting = true;
            Factory->ImportPackage(Next);
        }
    }
}

void AssetManager::ImportComplete(PACKAGE_HANDLE Package, bool bSuccess)
{
    bIsImporting = false;

    if (!bSuccess)
    {
        DeletePackage(Package);
    }

    ImportNextPackage();
}

PACKAGE_HANDLE AssetManager::CreatePackage(const std::string& PackageName, const std::string& Source)
{
    unsigned int Index = PackageIDs->Add();
    AssetPackage* Package = CreateObject<AssetPackage>(this);
    Packages[Index] = Package;

    PACKAGE_HANDLE Handle = (Index << 0);
    Package->Handle = Handle;
    Package->PackageName = PackageName;
    Package->Source = Source;

    return Handle;
}

void AssetManager::DeletePackage(PACKAGE_HANDLE Package)
{
    if (!IsValidPackage(Package))
        return;

    AssetPackage* PackageObject = GetPackageObject(Package);

    for (int i = PackageObject->Assets.size() - 1; i >= 0; --i)
    {
        RemoveAsset(PackageObject->Assets[i]);
    }

    PackageObject->Destroy();

    unsigned int Index = (0x0000ffff & Package);
    Packages[Index] = nullptr;
}

ASSET_HANDLE AssetManager::AddAsset(AssetResource* Asset, PACKAGE_HANDLE Package)
{
    if (Asset == nullptr || !IsValidPackage(Package))
        return INVALID_ASSET_HANDLE;

    unsigned int Index = AssetIDs->Add();
    ASSET_HANDLE Handle = (Index << 0);
    Asset->Handle = Handle;
    Asset->OwningPackage = Package;
    Assets[Index] = Asset;

    AssetPackage* PackageObject = GetPackageObject(Package);
    PackageObject->Assets.push_back(Handle);
    return Handle;
}

void AssetManager::RemoveAsset(ASSET_HANDLE Asset)
{
    if (!IsValidAsset(Asset))
        return;

    unsigned int Index = (Asset & 0x0000ffff >> 0);
    AssetIDs->Remove(Index);

    AssetResource* AssetObject = GetAssetObject(Asset);
    AssetPackage* AssetPackage = GetPackageObject(AssetObject->OwningPackage);

    std::remove(AssetPackage->Assets.begin(), AssetPackage->Assets.end(), Asset);
    AssetObject->Handle = INVALID_ASSET_HANDLE;
}

AssetResource* AssetManager::GetAssetObject(ASSET_HANDLE Handle) const
{
    return Assets[(Handle & 0x0000ffff)];
}

AssetPackage* AssetManager::GetPackageObject(PACKAGE_HANDLE Handle) const
{
    return Packages[(Handle & 0x0000ffff)];
}

bool AssetManager::IsValidAsset(ASSET_HANDLE Handle) const
{
    return GetAssetObject(Handle) != nullptr;
}

bool AssetManager::IsValidPackage(PACKAGE_HANDLE Handle) const
{
    return GetPackageObject(Handle) != nullptr;
}

AssetFactory* AssetManager::GetAssetFactory(const std::string& Extension) const
{
    const auto Iter = Factories.find(Extension);
    return Iter == Factories.end() ? nullptr : Iter->second;
}
