#include "Shader.h"
#include <glad/glad.h>
#include "Utils/FileUtils.h"
#include <iostream>

void Shader::Init()
{
	Object::Init();
	
	ShaderIDs[static_cast<int>(ShaderTypes::Vertex)] = glCreateShader(GL_VERTEX_SHADER);
	ShaderIDs[static_cast<int>(ShaderTypes::Fragment)] = glCreateShader(GL_FRAGMENT_SHADER);
	ShaderIDs[static_cast<int>(ShaderTypes::TessControl)] = glCreateShader(GL_TESS_CONTROL_SHADER);
	ShaderIDs[static_cast<int>(ShaderTypes::TessEval)] = glCreateShader(GL_TESS_EVALUATION_SHADER);
}

void Shader::Shutdown()
{
	Object::Shutdown();
	
	glDeleteShader(ShaderIDs[static_cast<int>(ShaderTypes::Vertex)]);
	glDeleteShader(ShaderIDs[static_cast<int>(ShaderTypes::Fragment)]);
	glDeleteShader(ShaderIDs[static_cast<int>(ShaderTypes::TessControl)]);
	glDeleteShader(ShaderIDs[static_cast<int>(ShaderTypes::TessEval)]);
}

void Shader::SetShaderSource(ShaderTypes Type, const std::string& Source)
{
	std::string File;  
	
	if (FileUtils::LoadFileToString(Source, File))
	{
		ShaderSources[static_cast<unsigned int>(Type)] = Source;
		const char* CharPtr = File.c_str();
		HasShaderFlags |= (1 << static_cast<int>(Type));
		glShaderSource(ShaderIDs[static_cast<int>(Type)], 1, &CharPtr, 0);
		glCompileShader(ShaderIDs[static_cast<int>(Type)]);

		int Success;
		char InfoLog[512];
	
		// print compile errors if any
		glGetShaderiv(ShaderIDs[static_cast<int>(Type)], GL_COMPILE_STATUS, &Success);
		if(!Success)
		{
			glGetShaderInfoLog(ShaderIDs[static_cast<int>(Type)], 512, NULL, InfoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << InfoLog << std::endl;
		};
	}
	else
	{
		// Throw invalid file error
	}
}

void Shader::Compile()
{
	ProgramID = glCreateProgram();

	for(unsigned int i = 0; i < static_cast<unsigned int>(ShaderTypes::COUNT); ++i)
	{
		if(HasShaderType(static_cast<ShaderTypes>(i)))
		{
			glAttachShader(ProgramID, ShaderIDs[i]);
		}
	}

	glLinkProgram(ProgramID);
	CacheUniforms();
}

bool Shader::HasShaderType(ShaderTypes Type) const
{
	return (HasShaderFlags & (1 << (static_cast<int>(Type)))) != 0;
}

int Shader::GetUniformLocation(const std::string& Name) const
{
	auto iter = ParamCache.find(Name);

	if (iter == ParamCache.end())
	{
		return -1;
	}

	return iter->second.UniformLocation;
}

int Shader::GetUniformBlockIndex(const std::string& Name) const
{
	auto iter = UniformBlockCache.find(Name);

	if (iter == UniformBlockCache.end())
	{
		return -1;
	}

	return iter->second.BlockIndex;
}

void Shader::CacheUniforms()
{
	ParamCache.empty();

	glUseProgram(GetProgramID());

	GLint count;
	glGetProgramiv(GetProgramID(), GL_ACTIVE_UNIFORMS, &count);

	for (GLint i = 0; i < count; i++)
	{
		ShaderParamCache Param;
		GLint length;
		GLint size;
		glGetActiveUniform(GetProgramID(), (GLuint)i, 24, &length, &size, &Param.UniformType, Param.UniformName);
		Param.UniformLocation = glGetUniformLocation(GetProgramID(), Param.UniformName);

		std::string strName = std::string(Param.UniformName);
		ParamCache[strName] = Param;
	}

	glGetProgramiv(GetProgramID(), GL_ACTIVE_UNIFORM_BLOCKS, &count);

	for (GLint i = 0; i < count; ++i)
	{
		ShaderUniformBlockCache Param;
		GLint length;
		GLint size;
		glGetActiveUniformBlockName(GetProgramID(), i, 24, &length, Param.BlockName);
		Param.BlockIndex = glGetUniformBlockIndex(GetProgramID(), Param.BlockName);

		std::string strName = std::string(Param.BlockName);
		UniformBlockCache[strName] = Param;
	}
}

unsigned Shader::GetProgramID() const
{
	return ProgramID;
}
