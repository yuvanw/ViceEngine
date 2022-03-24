#version 330 core

in FVertexToFragment
{
	vec3 TextureCoordinates;
} Input;

out vec4 FragmentColor;

uniform samplerCube Skybox;

void main()
{
	FragmentColor = texture(Skybox, Input.TextureCoordinates);
}
