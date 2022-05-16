#include "Material.h"

#include <glad/glad.h>

#include "Shader.h"
#include <Texture.h>
#include <iostream>

#include "RenderManager.h"

void Material::Init()
{
    AssetResource::Init();

    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::FLOAT)] = sizeof(float);
    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC2)] = sizeof(glm::vec2);
    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC3)] = sizeof(glm::vec3);
    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC4)] = sizeof(glm::vec4);
    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::MAT4)] = sizeof(glm::mat4);
    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::TEXTURE)] = sizeof(UniformBufferObject*);
    ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::BUFFER)] = sizeof(unsigned int);

    SetBuffer("Camera", RenderManager::CameraBuffer);
}

void Material::Shutdown()
{
    AssetResource::Shutdown();
}

void Material::Use()
{
    GetRenderManager()->BindMaterial(this);
}

void Material::BindParameters()
{
    for(auto Iter : Values)
    {
        int Loc = glGetUniformLocation(GetProgramID(), Iter.first.c_str());

        switch(Iter.second.Type)
        {
        case ShaderParamTypes::FLOAT:
            glUniform1f(Loc, *(GLfloat*)(Iter.second.Value));
            break;

        case ShaderParamTypes::VEC2:
            glUniform2fv(Loc, 1, (GLfloat*)Iter.second.Value);
            break;

        case ShaderParamTypes::VEC3:
            glUniform3fv(Loc, 1, (GLfloat*)Iter.second.Value);
            break;

        case ShaderParamTypes::VEC4:
            glUniform4fv(Loc, 1, (GLfloat*)Iter.second.Value);
            break;

        case ShaderParamTypes::MAT4:
            glUniformMatrix4fv(Loc, 1, GL_FALSE, (GLfloat*)Iter.second.Value);
            break;
        }
    }
    
    BindTextures();
    BindBuffers();
}

bool Material::DoesParamExist(const std::string& Name) const
{
    return MatShader && glGetUniformLocation(GetProgramID(), Name.c_str()) != -1;
}

void Material::SetFloat(const std::string& Name, float Value)
{
    AddParameter(Name, ShaderParamTypes::FLOAT);

    memcpy(Values[Name].Value, &Value, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::FLOAT)]);
    unsigned int Loc = GetUniformLocation(Name);
}

void Material::SetVec2(const std::string& Name, const glm::vec2& Value)
{
    AddParameter(Name, ShaderParamTypes::VEC2);

    memcpy(Values[Name].Value, &Value, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC2)]);
    unsigned int Loc = GetUniformLocation(Name);
}

void Material::SetVec3(const std::string& Name, const glm::vec3& Value)
{
    AddParameter(Name, ShaderParamTypes::VEC3);

    memcpy(Values[Name].Value, &Value, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC3)]);
    unsigned int Loc = GetUniformLocation(Name);
}

void Material::SetVec4(const std::string& Name, const glm::vec4& Value)
{
    AddParameter(Name, ShaderParamTypes::VEC4);

    memcpy(Values[Name].Value, &Value, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC4)]);
    unsigned int Loc = GetUniformLocation(Name);
}

void Material::SetMat4(const std::string& Name, const glm::mat4& Value)
{
    AddParameter(Name, ShaderParamTypes::MAT4);

    memcpy(Values[Name].Value, &Value, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::MAT4)]);
    unsigned int Loc = GetUniformLocation(Name);
}

void Material::SetTexture(const std::string& Name, Texture* Value)
{
    AddParameter(Name, ShaderParamTypes::TEXTURE);
    memcpy(Values[Name].Value, &Value, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::TEXTURE)]);

    int Index = FindTextureParamIndex(Name);
    if (Index != -1)
    {
        Textures[Index] = Textures[NumTextureParams];
        TextureParamNames[Index] = TextureParamNames[NumTextureParams];
        NumTextureParams--;
    }

    if (Value != nullptr)
    {
        Textures[NumTextureParams] = Value;
        TextureParamNames[NumTextureParams] = Name;
        NumTextureParams++;
    }
}

void Material::SetBuffer(const std::string& Name, UniformBufferObject* Value)
{
    AddParameter(Name, ShaderParamTypes::BUFFER);
    memcpy(Values[Name].Value, &Value, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::BUFFER)]);

    int Index = FindBufferParamIndex(Name);
    if (Index != -1)
    {
        Buffers[Index] = Buffers[NumBuffers];
        BufferParamNames[Index] = BufferParamNames[NumBuffers];
        NumBuffers--;
    }

    if (Value != nullptr)
    {
        Buffers[NumBuffers] = Value;
        BufferParamNames[NumBuffers] = Name;
        NumBuffers++;
    }
}

bool Material::GetFloat(const std::string& Name, float& OutValue) const
{
    void* ParamValue;

    if (!GetParameter(Name, ParamValue))
        return false;

    memcpy(&OutValue, ParamValue, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::FLOAT)]);
    return true;
}

bool Material::GetVec2(const std::string& Name, glm::vec2& OutValue) const
{
    void* ParamValue;

    if (!GetParameter(Name, ParamValue))
        return false;

    memcpy(&OutValue, ParamValue, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC2)]);
    return true;
}

bool Material::GetVec3(const std::string& Name, glm::vec3& OutValue) const
{
    void* ParamValue;

    if (!GetParameter(Name, ParamValue))
        return false;

    memcpy(&OutValue, ParamValue, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC3)]);
    return true;
}

bool Material::GetVec4(const std::string& Name, glm::vec4& OutValue) const
{
    void* ParamValue;

    if (!GetParameter(Name, ParamValue))
        return false;

    memcpy(&OutValue, ParamValue, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::VEC4)]);
    return true;
}

bool Material::GetMat4(const std::string& Name, glm::mat4& OutValue) const
{
    void* ParamValue;

    if (!GetParameter(Name, ParamValue))
        return false;

    memcpy(&OutValue, ParamValue, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::MAT4)]);
    return true;
}

bool Material::GetTexture(const std::string& Name, Texture*& OutValue) const
{
    int Index = FindTextureParamIndex(Name);

    if (Index != -1)
    {
        OutValue = Textures[Index];
    }

    return Index != -1;
}

bool Material::GetBuffer(const std::string& Name, UniformBufferObject*& OutValue) const
{
    void* ParamValue;

    if (!GetParameter(Name, ParamValue))
        return false;

    memcpy(&OutValue, ParamValue, ParamSizesBytes[static_cast<unsigned int>(ShaderParamTypes::BUFFER)]);
    return true;
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
    for (unsigned int i = 0; i < NumTextureParams; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, Textures[i]->GetTextureID());

        unsigned int Loc = GetUniformLocation(TextureParamNames[i]);
        glUniform1i(Loc, i);
    }
}

void Material::BindBuffers()
{
    for(unsigned int i = 0; i < NumBuffers; ++i)
    {
        GLuint Loc = glGetUniformBlockIndex(GetProgramID(), BufferParamNames[i].c_str());
        glUniformBlockBinding(GetProgramID(), Loc, Buffers[i]->GetBoundIndex());
    }
}

void Material::SetShader(Shader* NewShader)
{
    MatShader = NewShader;
}

Shader* Material::GetShader() const
{
    return MatShader;
}

unsigned Material::GetProgramID() const
{
    return MatShader->GetProgramID();
}

int Material::GetUniformLocation(const std::string& Name) const
{
    return glGetUniformLocation(GetProgramID(), Name.c_str());
}

void Material::MaterialBound()
{

}

void Material::MaterialUnbound()
{
}

void Material::AddParameter(const std::string& Name, ShaderParamTypes Type)
{
    ShaderParam Param;
    Param.Type = Type;
    Param.Value = new unsigned char[ParamSizesBytes[static_cast<unsigned int>(Type)]];
    Values[Name] = Param;
}

bool Material::GetParameter(const std::string& Name, void*& OutValue) const
{
    auto Iter = Values.find(Name);
    if (Iter == Values.end())
        return false;

    OutValue = Iter->second.Value;
    return true;
}

int Material::FindTextureParamIndex(const std::string& Name) const
{
    for (size_t i = 0; i < NumTextureParams; ++i)
    {
        if (Name == TextureParamNames[i])
        {
            return i;
        }
    }

    return -1;
}

int Material::FindBufferParamIndex(const std::string& Name) const
{
    for (size_t i = 0; i < NumBuffers; ++i)
    {
        if (Name == BufferParamNames[i])
        {
            return i;
        }
    }

    return -1;
}
