#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "LookUpTable.h"
#include "Shader.h"
#include "Assets/AssetManager.h"
#include "Core/Engine.h"
#include "Core/Object.h"

class Texture;
class Shader;

class Material : public AssetResource
{
    enum class ShaderParamTypes
    {
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        MAT4,
        TEXTURE,
        COUNT
    };
    
    struct ShaderParam
    {
    public:
        ShaderParamTypes Type;
        void* Value;
    };
    
public:
    Material(Object* NewOuter);
    ~Material();
    
    bool DoesParamExist(const std::string& Name) const;

    void SetFloat(const std::string& Name, float Value);
    void SetVec2(const std::string& Name, const glm::vec2& Value);
    void SetVec3(const std::string& Name, const glm::vec3& Value);
    void SetVec4(const std::string& Name, const glm::vec4& Value);
    void SetMat4(const std::string& Name, const glm::mat4& Value);
    void SetTexture(const std::string& Name, Texture* Value);

    bool GetFloat(const std::string& Name, float& OutValue) const;
    bool GetVec2(const std::string& Name, glm::vec2& OutValue) const;
    bool GetVec3(const std::string& Name, glm::vec3& OutValue) const;
    bool GetVec4(const std::string& Name, glm::vec4& OutValue) const;
    bool GetMat4(const std::string& Name, glm::mat4& OutValue) const;
    bool GetTexture(const std::string& Name, Texture*& OutValue) const;

    void SetName(const std::string& NewName);
    std::string GetName() const;
    void SetShader(Shader* NewShader);
    Shader* GetShader() const;

    void Use();

    unsigned int GetProgramID() const;
    int GetUniformLocation(const std::string& Name) const;

private:
    bool AddParameter(const std::string& Name, ShaderParamTypes Type);
    bool GetParameter(const std::string& Name, void*& OutValue) const;
    int FindTextureParamIndex(const std::string& Name) const;
    void BindTextures();

    unsigned int ProgramID;
    
    Shader* MatShader;
    std::string MatName;
    
    Texture* Textures[32];
    std::string TextureParamNames[32];
    unsigned int NumTextureParams = 0;

    std::unordered_map<std::string, ShaderParam> Values;
    unsigned int ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::COUNT)];
};
