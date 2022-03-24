#version 330 core

layout (location = 0) in vec3 InPosition;

out FVertexToFragment
{
	vec3 TextureCoordinates;
} Output;

struct FMatrices
{
	mat4 View;
	mat4 Projection;
};

uniform FMatrices Matrices;

void main()
{
	Output.TextureCoordinates = InPosition;
	gl_Position = Matrices.Projection * Matrices.View * vec4(InPosition, 1.0);
}
