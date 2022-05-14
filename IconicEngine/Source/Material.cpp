#include "Material.h"

#include <glad/glad.h>

#include "Shader.h"
#include <Texture.h>
#include <iostream>

Material::Material(Object* NewOuter) : AssetResource(NewOuter)
{
    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::FLOAT)] = sizeof(float);
    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC2)] = sizeof(glm::vec2);
    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC3)] = sizeof(glm::vec3);
    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC4)] = sizeof(glm::vec4);
    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::MAT4)] = sizeof(glm::mat4);
    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::TEXTURE)] = sizeof(Texture*);
}

Material::~Material()
{
}

bool Material::DoesParamExist(const std::string& Name) const
{
    return MatShader && glGetUniformLocation(GetProgramID(), Name.c_str()) != -1;
}

void Material::Use()
{
    glUseProgram(GetProgramID());
    BindTextures();
}

void Material::SetFloat(const std::string& Name, float Value)
{
    if(!AddParameter(Name, ShaderParamTypes::FLOAT))
        return;
    
    memcpy(Values[Name].Value, &Value, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::FLOAT)]);
    unsigned int Loc = GetUniformLocation(Name);
    glUseProgram(GetProgramID());
    glUniform1fv(Loc, 1, static_cast<GLfloat*>(Values[Name].Value));
}

void Material::SetVec2(const std::string& Name, const glm::vec2& Value)
{
    if(!AddParameter(Name, ShaderParamTypes::VEC2))
        return;
    
    memcpy(Values[Name].Value, &Value, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC2)]);
    unsigned int Loc = GetUniformLocation(Name);
    glUseProgram(GetProgramID());
    glUniform2fv(Loc, 1, static_cast<GLfloat*>(Values[Name].Value));
}

void Material::SetVec3(const std::string& Name, const glm::vec3& Value)
{
    if(!AddParameter(Name, ShaderParamTypes::VEC3))
        return;
    
    memcpy(Values[Name].Value, &Value, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC3)]);
    unsigned int Loc = GetUniformLocation(Name);
    glUseProgram(GetProgramID());
    glUniform3fv(Loc, 1, static_cast<GLfloat*>(Values[Name].Value));
}

void Material::SetVec4(const std::string& Name, const glm::vec4& Value)
{
    if(!AddParameter(Name, ShaderParamTypes::VEC4))
        return;
    
    memcpy(Values[Name].Value, &Value, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC4)]);
    unsigned int Loc = GetUniformLocation(Name);
    glUseProgram(GetProgramID());
    glUniform4fv(Loc, 1, static_cast<GLfloat*>(Values[Name].Value));
}

void Material::SetMat4(const std::string& Name, const glm::mat4& Value)
{
    if(!AddParameter(Name, ShaderParamTypes::MAT4))
        return;

    memcpy(Values[Name].Value, &Value, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::MAT4)]);
    unsigned int Loc = GetUniformLocation(Name);
    glUseProgram(GetProgramID()); 
    glUniformMatrix4fv(Loc, 1, GL_FALSE, static_cast<const GLfloat*>(Values[Name].Value));
}

void Material::SetTexture(const std::string& Name, Texture* Value)
{
    if (!AddParameter(Name, ShaderParamTypes::TEXTURE))
        return;

    int Index = FindTextureParamIndex(Name);
    if(Index != -1)
    {
        Textures[Index] = Textures[NumTextureParams];
        TextureParamNames[Index] = TextureParamNames[NumTextureParams];
        NumTextureParams--;
    }

    if(Value != nullptr)
    {
        Textures[NumTextureParams] = Value;
        TextureParamNames[NumTextureParams] = Name;
        NumTextureParams++;   
    }

    for(unsigned int i = 0; i < NumTextureParams; ++i)
    {
        glUseProgram(GetProgramID());
        unsigned int Loc = GetUniformLocation(TextureParamNames[i]);
        glUniform1i(Loc, i);
    }
}

bool Material::GetFloat(const std::string& Name, float& OutValue) const
{
    void* ParamValue;
    
    if(!GetParameter(Name, ParamValue))
        return false;

    memcpy(&OutValue, ParamValue, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::FLOAT)]);
    return true;
}

bool Material::GetVec2(const std::string& Name, glm::vec2& OutValue) const
{
    void* ParamValue;
    
    if(!GetParameter(Name, ParamValue))
        return false;

    memcpy(&OutValue, ParamValue, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC2)]);
    return true;
}

bool Material::GetVec3(const std::string& Name, glm::vec3& OutValue) const
{
    void* ParamValue;
    
    if(!GetParameter(Name, ParamValue))
        return false;

    memcpy(&OutValue, ParamValue, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC3)]);
    return true;
}

bool Material::GetVec4(const std::string& Name, glm::vec4& OutValue) const
{
    void* ParamValue;
    
    if(!GetParameter(Name, ParamValue))
        return false;

    memcpy(&OutValue, ParamValue, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC4)]);
    return true;
}

bool Material::GetMat4(const std::string& Name, glm::mat4& OutValue) const
{
    void* ParamValue;
    
    if(!GetParameter(Name, ParamValue))
        return false;

    memcpy(&OutValue, ParamValue, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::MAT4)]);
    return true;
}

bool Material::GetTexture(const std::string& Name, Texture*& OutValue) const
{
    int Index = FindTextureParamIndex(Name);

    if(Index != -1)
    {
        OutValue = Textures[Index];
    }

    return Index != -1;
}

void Material::SetName(const std::string& NewName)
{
    MatName = NewName;
}

std::string Material::GetName() const
{
    return MatName;
}

void Material::BindTextures()
{
    for(size_t i = 0; i < NumTextureParams; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, Textures[i]->GetTextureID());
    }
}

void Material::SetShader(Shader* NewShader)
{
    MatShader = NewShader;
    ProgramID = NewShader->CreateProgram();
}

Shader* Material::GetShader() const
{
    return MatShader;
}

unsigned Material::GetProgramID() const
{
    return ProgramID;
}

int Material::GetUniformLocation(const std::string& Name) const
{
    return glGetUniformLocation(GetProgramID(), Name.c_str());
}

bool Material::AddParameter(const std::string& Name, ShaderParamTypes Type)
{
    if(Values.find(Name) == Values.end())
    {
        ShaderParam Param;
        Param.Type = Type;
        Param.Value = new char[ParamSizesBytes[static_cast<unsigned int>(Type)]];
        Values[Name] = Param;
        return true;
    }
    else if(Values[Name].Type != Type)
    {
        return false;
    }

    return true;
}

bool Material::GetParameter(const std::string& Name, void*& OutValue) const
{
    auto Iter = Values.find(Name);
    if(Iter == Values.end())
        return false;
    
    OutValue = Iter->second.Value;
    return true;
}

int Material::FindTextureParamIndex(const std::string& Name) const
{
    for(size_t i = 0; i < NumTextureParams; ++i)
    {
        if(Name == TextureParamNames[i])
        {
            return i;
        }
    }

    return -1;
}
