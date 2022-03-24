#version 330 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InNormal;
layout (location = 2) in vec2 InTextureCoordinates;

out FVertexToFragment
{
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoordinates;
} Output;

struct FMatrices
{
	mat4 Model;
	mat4 View;
	mat4 Projection;
};

uniform FMatrices Matrices;

void main()
{
	Output.Position = vec3(Matrices.Model * vec4(InPosition, 1.0));
    Output.Normal = mat3(transpose(inverse(Matrices.Model))) * InNormal;
	Output.TextureCoordinates = InTextureCoordinates;
    
    gl_Position = Matrices.Projection * Matrices.View * Matrices.Model * vec4(InPosition, 1.0);
}
