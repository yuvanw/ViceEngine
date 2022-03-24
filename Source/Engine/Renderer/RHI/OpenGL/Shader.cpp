#include "Shader.h"
#include "RendererFileSystem.h"

#include <string>
#include <fstream>
#include <sstream>

FShader::FShader(const FStringId& InShaderFileName, EShaderType InShaderType)
{
	FStringId ShaderFilePath = FRendererFileSystem::GetShaderFilePath(InShaderFileName);
	
	// Open shader file.
	std::ifstream ShaderFileStream(ShaderFilePath.GetString().GetData());
	ensure(ShaderFileStream);

	// Read shader source code.
	std::stringstream ShaderBuffer;
	ShaderBuffer << ShaderFileStream.rdbuf();
	std::string ShaderString = ShaderBuffer.str();
	const ANSICHAR* ShaderCString = ShaderString.c_str();

	// Create shader object and compile shader source code.
	Id = glCreateShader(static_cast<GLenum>(InShaderType));
	glShaderSource(Id, 1, &ShaderCString, NULL);
	glCompileShader(Id);
	int32 IsCompiled;
	glGetShaderiv(Id, GL_COMPILE_STATUS, &IsCompiled);
	ensure(IsCompiled);
}

FShader::~FShader()
{
	glDeleteShader(Id);
}
