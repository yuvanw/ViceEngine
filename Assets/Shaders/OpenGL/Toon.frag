#version 330 core

// Toon shader logic referenced from:
// https://roystan.net/articles/toon-shader.html

struct FMaterial
{
	vec3 Diffuse;
	float Shininess;
};

struct FDirectionalLight
{
	vec3 Direction;
	vec3 Color;
	float Intensity;
};

struct FAttenuation
{
	float Constant;
	float Linear;
	float Quadratic;
};

struct FPointLight
{
	vec3 Position;
	vec3 Color;
	FAttenuation Attenuation;
	float Intensity;
};

in FVertexToFragment
{
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoordinates;
} Input;

out vec4 FragmentColor;

uniform vec3 CameraPosition;
uniform FMaterial Material;

#define MAX_DIRECTIONAL_LIGHTS 10
uniform int NumDirectionalLights;
uniform FDirectionalLight DirectionalLights[MAX_DIRECTIONAL_LIGHTS];

#define MAX_POINT_LIGHTS 10
uniform int NumPointLights;
uniform FPointLight PointLights[MAX_POINT_LIGHTS];

vec3 CalculateDirectionalLighting(FDirectionalLight InLight, vec3 InNormal, vec3 InCameraDirection)
{
	// Ambient
    vec3 Ambient = InLight.Color * Material.Diffuse;
  	
    // Diffuse 
	vec3 LightDirection = normalize(-InLight.Direction);
    float DiffuseStrength = max(dot(InNormal, LightDirection), 0.0);
    DiffuseStrength = smoothstep(0, 0.05, DiffuseStrength);
	vec3 Diffuse = InLight.Color * DiffuseStrength * Material.Diffuse;
    
    // Specular
	vec3 HalfwayDirection = normalize(LightDirection + InCameraDirection);
    float SpecularStrength = pow(max(dot(InNormal, HalfwayDirection), 0.0), Material.Shininess);
    SpecularStrength = smoothstep(0.01, 0.02, SpecularStrength);
	vec3 Specular = InLight.Color * SpecularStrength;
    
	// Rim
	float RimStrength = 1.0 - max(dot(InCameraDirection, InNormal), 0.0);
	RimStrength = smoothstep(0.70, 0.71, RimStrength);
	vec3 Rim = InLight.Color * RimStrength;

    return (Ambient + Diffuse + Specular + Rim) * InLight.Intensity;
}

vec3 CalculatePointLighting(FPointLight InLight, vec3 InNormal, vec3 InCameraDirection, vec3 InFragmentPosition)
{
	// Ambient
    vec3 Ambient = InLight.Color * Material.Diffuse;
  	
    // Diffuse 
    vec3 LightDirection = normalize(InLight.Position - InFragmentPosition);
    float DiffuseStrength = max(dot(InNormal, LightDirection), 0.0);
    DiffuseStrength = smoothstep(0, 0.05, DiffuseStrength);
    vec3 Diffuse = InLight.Color * DiffuseStrength * Material.Diffuse;
    
    // Specular
	vec3 HalfwayDirection = normalize(LightDirection + InCameraDirection);
    float SpecularStrength = pow(max(dot(InNormal, HalfwayDirection), 0.0), Material.Shininess);
	SpecularStrength = smoothstep(0.01, 0.02, SpecularStrength);
    vec3 Specular = InLight.Color * SpecularStrength;
    
	// Rim
	float RimStrength = 1.0 - max(dot(InCameraDirection, InNormal), 0.0);
	RimStrength = smoothstep(0.70, 0.71, RimStrength);
	vec3 Rim = InLight.Color * RimStrength;

	// Attenuation
	float Distance = length(InLight.Position - InFragmentPosition);
	float Attenuation = 1.0 / (InLight.Attenuation.Constant + (InLight.Attenuation.Linear * Distance) + (InLight.Attenuation.Quadratic * (Distance * Distance)));

	Ambient *= Attenuation;
	Diffuse *= Attenuation;
	Specular *= Attenuation;
	Rim *= Attenuation;

	return (Ambient + Diffuse + Specular + Rim) * InLight.Intensity;
}

void main()
{
    vec3 Normal = normalize(Input.Normal);
    vec3 CameraDirection = normalize(CameraPosition - Input.Position);

	vec3 FinalColor = vec3(0.0);
	for (int Index = 0; Index < NumDirectionalLights; ++Index)
	{
		FinalColor += CalculateDirectionalLighting(DirectionalLights[Index], Normal, CameraDirection);
	}

	for (int Index = 0; Index < NumPointLights; ++Index)
	{
		FinalColor += CalculatePointLighting(PointLights[Index], Normal, CameraDirection, Input.Position);
	}

	FragmentColor = vec4(FinalColor, 1.0);
}
