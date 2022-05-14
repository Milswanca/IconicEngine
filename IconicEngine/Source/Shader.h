#pragma once
#include <string>
#include <glm/detail/type_mat4x2.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "Core/Object.h"

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
public:
    Shader(Object* NewOuter);
    virtual ~Shader();
    
    virtual void Init() override;
    virtual void Shutdown() override;

    void SetShaderSource(ShaderTypes Type, const std::string& Source);
    unsigned int CreateProgram();

    bool HasShaderType(ShaderTypes Type) const;

private:
    int HasShaderFlags = 0;

    unsigned int ShaderIDs[static_cast<int>(ShaderTypes::COUNT)];
    std::string ShaderSources[static_cast<int>(ShaderTypes::COUNT)];
};
