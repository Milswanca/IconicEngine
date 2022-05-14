#pragma once
#include "AssetTypes.h"
#include "Core/Object.h"

#include <string>

class AssetFactory : public Object
{
public:
    AssetFactory(Object* NewOuter);
    virtual ~AssetFactory();

protected:
    virtual void ImportPackage(PACKAGE_HANDLE Package);
    virtual void OnImportSuccess();
    virtual void OnImportFailed();

    void AddSupportedFileExtension(const std::string& Extension);
    void RemoveSupportedFileExtension(const std::string& Extension);
    void GetSupportedFileExtensions(std::string& OutExtensions) const;
    void ImportComplete(PACKAGE_HANDLE Package, bool bSuccess);
    
private:
    std::string SupportedExtensions;
    
    friend class AssetManager;
};
