#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Utils/LookUpTable.h"
#include "Shader.h"
#include "UniformBufferObject.h"
#include "Assets/AssetManager.h"
#include "Core/Engine.h"
#include "Core/Object.h"

class Texture;
class Shader;

class Material : public AssetResource
{
    enum class ShaderParamTypes
    {
        INT,
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        MAT4,
        COUNT
    };
    
    struct ShaderParam
    {
    public:
        ShaderParamTypes Type;
        unsigned char* Value;
    };
    
public:
    IMPLEMENT_CONSTRUCTOR(Material, AssetResource);
    
    virtual void Init() override;
    virtual void Shutdown() override;

    void Use();
    
    bool DoesParamExist(const std::string& Name) const;

	void SetInt(const std::string& Name, int Value);
	void SetFloat(const std::string& Name, float Value);
    void SetVec2(const std::string& Name, const glm::vec2& Value);
    void SetVec3(const std::string& Name, const glm::vec3& Value);
    void SetVec4(const std::string& Name, const glm::vec4& Value);
    void SetMat4(const std::string& Name, const glm::mat4& Value);
    void SetTexture(const std::string& Name, Texture* Value);
    void SetBuffer(const std::string& Name, UniformBufferObject* Value);

	bool GetInt(const std::string& Name, int& OutValue) const;
	bool GetFloat(const std::string& Name, float& OutValue) const;
    bool GetVec2(const std::string& Name, glm::vec2& OutValue) const;
    bool GetVec3(const std::string& Name, glm::vec3& OutValue) const;
    bool GetVec4(const std::string& Name, glm::vec4& OutValue) const;
    bool GetMat4(const std::string& Name, glm::mat4& OutValue) const;
    bool GetTexture(const std::string& Name, Texture*& OutValue) const;
    bool GetBuffer(const std::string& Name, UniformBufferObject*& OutValue) const;

    void SetName(const std::string& NewName);
    std::string GetName() const;
    void SetShader(Shader* NewShader);
    Shader* GetShader() const;

    unsigned int GetProgramID() const;
	int GetUniformLocation(const std::string& Name) const;
	int GetUniformBlockIndex(const std::string& Name) const;

protected:
    virtual void MaterialBound();
    virtual void MaterialUnbound();
    
private:
    void AddParameter(const std::string& Name, ShaderParamTypes Type);
    bool GetParameter(const std::string& Name, void*& OutValue) const;
    
    int FindTextureParamIndex(const std::string& Name) const;
    int FindBufferParamIndex(const std::string& Name) const;
    
	void BindParameters();
	void UnbindParameters();

    void BindTextures();
    void BindBuffers();

    void UnbindTextures();
    void UnbindBuffers();

    Shader* MatShader;
    std::string MatName;
    
    Texture* Textures[32];
    std::string TextureParamNames[32];
    unsigned int NumTextureParams = 0;

    UniformBufferObject* Buffers[32];
    std::string BufferParamNames[32];
    unsigned int NumBuffers = 0;

    std::unordered_map<std::string, ShaderParam> Values;
    unsigned int ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::COUNT)];

    friend class RenderManager;
};
