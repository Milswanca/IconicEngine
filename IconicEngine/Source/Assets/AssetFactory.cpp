#include "AssetFactory.h"
#include "AssetManager.h"
#include "Core/Engine.h"
#include "Utils/StringUtils.h"

void AssetFactory::ImportPackage(PACKAGE_HANDLE Package)
{
    ImportComplete(Package, false);
}

void AssetFactory::OnImportSuccess()
{
    
}

void AssetFactory::OnImportFailed()
{
    
}

void AssetFactory::AddSupportedFileExtension(const std::string& Extension)
{
    // Probably dont need a split and join here but its easier to read
    std::vector<std::string> Extensions = StringUtils::Split(SupportedExtensions);
    Extensions.push_back(Extension);
    SupportedExtensions = StringUtils::Join(Extensions);
}

void AssetFactory::RemoveSupportedFileExtension(const std::string& Extension)
{
    std::vector<std::string> Extensions = StringUtils::Split(SupportedExtensions);
    std::remove(Extensions.begin(), Extensions.end(), Extension);
    SupportedExtensions = StringUtils::Join(Extensions);
}

void AssetFactory::GetSupportedFileExtensions(std::string& OutExtensions) const
{
    OutExtensions = SupportedExtensions;
}

void AssetFactory::ImportComplete(PACKAGE_HANDLE Package, bool bSuccess)
{
    if(bSuccess)
    {
        OnImportSuccess();
    }
    else
    {
        OnImportFailed();
    }

    Engine::Get()->GetAssetManager()->ImportComplete(Package, bSuccess);
}
