#include "Pipeline.h"

#include "Math/Vector2D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/Matrix4D.h"

FPipeline::FPipeline(const FShader& InVertexShader, const FShader& InFragmentShader)
	: Id(glCreateProgram())
{
	glAttachShader(Id, InVertexShader.GetId());
	glAttachShader(Id, InFragmentShader.GetId());
	glLinkProgram(Id);

	int32 IsLinked;
	glGetProgramiv(Id, GL_LINK_STATUS, &IsLinked);
	ensure(IsLinked);
}

FPipeline::FPipeline(const FShader& InVertexShader, const FShader& InFragmentShader, const FShader& InGeometryShader)
{
	Id = glCreateProgram();
	glAttachShader(Id, InVertexShader.GetId());
	glAttachShader(Id, InFragmentShader.GetId());
	glAttachShader(Id, InGeometryShader.GetId());
	glLinkProgram(Id);

	int32 IsLinked;
	glGetProgramiv(Id, GL_LINK_STATUS, &IsLinked);
	ensure(IsLinked);
}

FPipeline::~FPipeline()
{
	glDeleteProgram(Id);
}

void FPipeline::Bind() const
{
	glUseProgram(Id);
}

void FPipeline::SetBool(const FStringId& InUniformName, bool InBool)
{
	glUniform1i(glGetUniformLocation(Id, InUniformName.GetString().GetData()), (int32)InBool);
}

void FPipeline::SetInt(const FStringId& InUniformName, int32 InInt)
{
	glUniform1i(glGetUniformLocation(Id, InUniformName.GetString().GetData()), InInt);
}

void FPipeline::SetFloat(const FStringId& InUniformName, float InFloat)
{
	glUniform1f(glGetUniformLocation(Id, InUniformName.GetString().GetData()), InFloat);
}

void FPipeline::SetVector2D(const FStringId& InUniformName, const FVector2D& InVector2D)
{
	glUniform2fv(glGetUniformLocation(Id, InUniformName.GetString().GetData()), 1, &InVector2D[0]);
}

void FPipeline::SetVector3D(const FStringId& InUniformName, const FVector3D& InVector3D)
{
	uint32 Location = glGetUniformLocation(Id, InUniformName.GetString().GetData());
	glUniform3fv(glGetUniformLocation(Id, InUniformName.GetString().GetData()), 1, &InVector3D[0]);
}

void FPipeline::SetVector4D(const FStringId& InUniformName, const FVector4D& InVector4D)
{
	glUniform4fv(glGetUniformLocation(Id, InUniformName.GetString().GetData()), 1, &InVector4D[0]);
}

void FPipeline::SetMatrix4D(const FStringId& InUniformName, const FMatrix4D& InMatrix4D)
{
	glUniformMatrix4fv(glGetUniformLocation(Id, InUniformName.GetString().GetData()), 1, GL_FALSE, InMatrix4D.GetData());
}
