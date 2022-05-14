#include "Shader.h"
#include <glad/glad.h>
#include "Utils/FileUtils.h"
#include <iostream>

Shader::Shader(Object* NewOuter) : Object(NewOuter)
{
	ShaderIDs[static_cast<int>(ShaderTypes::Vertex)] = glCreateShader(GL_VERTEX_SHADER);
	ShaderIDs[static_cast<int>(ShaderTypes::Fragment)] = glCreateShader(GL_FRAGMENT_SHADER);
	ShaderIDs[static_cast<int>(ShaderTypes::TessControl)] = glCreateShader(GL_TESS_CONTROL_SHADER);
	ShaderIDs[static_cast<int>(ShaderTypes::TessEval)] = glCreateShader(GL_TESS_EVALUATION_SHADER);
}

Shader::~Shader()
{
	glDeleteShader(ShaderIDs[static_cast<int>(ShaderTypes::Vertex)]);
	glDeleteShader(ShaderIDs[static_cast<int>(ShaderTypes::Fragment)]);
	glDeleteShader(ShaderIDs[static_cast<int>(ShaderTypes::TessControl)]);
	glDeleteShader(ShaderIDs[static_cast<int>(ShaderTypes::TessEval)]);
}

void Shader::Init()
{
	Object::Init();
}

void Shader::Shutdown()
{
	Object::Shutdown();
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

unsigned int Shader::CreateProgram()
{
	unsigned int Program = glCreateProgram();

	for(unsigned int i = 0; i < static_cast<unsigned int>(ShaderTypes::COUNT); ++i)
	{
		if(HasShaderType(static_cast<ShaderTypes>(i)))
		{
			glAttachShader(Program, ShaderIDs[i]);
		}
	}
	int err = glGetError();

	glLinkProgram(Program);
	return Program;
}


bool Shader::HasShaderType(ShaderTypes Type) const
{
	return (HasShaderFlags & (1 << (static_cast<int>(Type)))) != 0;
}
