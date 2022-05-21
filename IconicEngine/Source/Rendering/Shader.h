#pragma once
#include <string>
#include <glm/detail/type_mat4x2.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "Core/Object.h"
#include <glad/glad.h>
#include <unordered_map>

enum class ShaderTypes
{
    Unknown,
    Vertex,
    Fragment,
    TessControl,
    TessEval,
    COUNT
};

class Shader : public Object
{
	struct ShaderParamCache
	{
		GLchar UniformName[24];
		GLint UniformLocation;
		GLenum UniformType;
	};

	struct ShaderUniformBlockCache
	{
		GLchar BlockName[24];
		GLint BlockIndex;
	};

public:
    IMPLEMENT_CONSTRUCTOR(Shader, Object);
    
    virtual void Init() override;
    virtual void Shutdown() override;

    void SetShaderSource(ShaderTypes Type, const std::string& Source);
    void Compile();

    unsigned int GetProgramID() const;
    bool HasShaderType(ShaderTypes Type) const;

	int GetUniformLocation(const std::string& Name) const;
	int GetUniformBlockIndex(const std::string& Name) const;

private:
    void CacheUniforms();

    int HasShaderFlags = 0;
    unsigned int ProgramID;

    unsigned int ShaderIDs[static_cast<int>(ShaderTypes::COUNT)];
    std::string ShaderSources[static_cast<int>(ShaderTypes::COUNT)];

	std::unordered_map<std::string, ShaderParamCache> ParamCache;
	std::unordered_map<std::string, ShaderUniformBlockCache> UniformBlockCache;
};
