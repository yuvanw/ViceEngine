#include "ForwardRenderer.h"

#include "CoreMinimal.h"
#include "RHI/RHI.h"
#include "Camera/Camera.h"
#include "Materials/Material.h"
#include "Geometry/Mesh.h"
#include "Geometry/Model.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"

#include <string>

// Helper functions for updating shader uniform data.
static void UpdateMaterialUniforms(FMaterial& InMaterial);
static void UpdateMatrixUniforms(const TSharedPtr<FPipeline>& InPipeline, const TSharedPtr<FModel>& InModel, const TSharedPtr<FCamera>& InCamera);
static void UpdateCameraUniforms(const TSharedPtr<FPipeline>& InPipeline, const TSharedPtr<FCamera>& InCamera);
static void UpdateLightUniforms(const TSharedPtr<FPipeline>& InPipeline, const TSharedPtr<FScene>& InScene);

void FForwardRenderer::Render()
{
	if (!Scene)
	{
		return;
	}

	RenderTarget->Bind();

	FRHI::EnableDepthTesting();
	FRHI::ClearColorBuffer();
	FRHI::ClearDepthBuffer();

	RenderSkybox();

	for (const TSharedPtr<FModel>& Model : Scene->GetVisibleModels())
	{
		RenderModel(Model);
	}

	RenderTarget->Unbind();

	FRHI::SwapBuffers();
}

void FForwardRenderer::RenderSkybox()
{
	const TSharedPtr<FSkybox>& Skybox = Scene->GetSkybox();
	if (!Skybox || !Skybox->IsVisible())
	{
		return;
	}

	// Make sure skybox is always fully rendered (i.e. not as a wireframe or as points).
	FRHI::SetDrawingMode(EDrawingMode::Filled);
	// Disable depth buffer writing while the skybox is being rendered,
	// so that other scene objects always appear in front of the skybox.
	FRHI::DisableDepthBufferWriting();

	TSharedPtr<FPipeline> SkyboxPipeline = Skybox->GetMesh().GetMaterial().GetPipeline();
	SkyboxPipeline->Bind();

	// Remove translation component from the view matrix so that the skybox appears to stretch out infinitely.
	FTransform4D ViewTransform = Camera->GetLookAt();
	ViewTransform.SetTranslation(FVector3D::Zero);
	SkyboxPipeline->SetMatrix4D(FUniformNames::ViewMatrix, ViewTransform.ToMatrix());

	// Keep frustum near/far plane distances constant so that the skybox is never clipped.
	FFrustum Frustum = Camera->GetFrustum();
	Frustum.NearDistance = 0.1f;
	Frustum.FarDistance = 2.0f;

	FMatrix4D Projection;
	if (Camera->GetProjectionType() == EProjectionType::Perspective)
	{
		Projection = FCamera::MakePerspectiveProjection(Frustum);
	}
	else
	{
		Projection = FCamera::MakePerspectiveProjection(Frustum);
	}
	SkyboxPipeline->SetMatrix4D(FUniformNames::ProjectionMatrix, Projection);

	Skybox->GetCubemap().Bind();

	const TSharedPtr<TVertexArray<FVertex1P1N1UV> >& SkyboxVertexArray = Skybox->GetMesh().GetVertexArray();
	SkyboxVertexArray->Bind();

	int32 NumIndices = SkyboxVertexArray->GetNumIndices();
	if (NumIndices > 0)
	{
		FRHI::DrawIndexed(NumIndices);
	}
	else
	{
		FRHI::Draw(SkyboxVertexArray->GetNumVertices());
	}

	FRHI::EnableDepthBufferWriting();
}

void FForwardRenderer::RenderModel(const TSharedPtr<FModel>& InModel)
{
	FRHI::SetDrawingMode(InModel->GetDrawingMode());

	for (FMesh& Mesh : InModel->GetMeshes())
	{
		FMaterial& Material = Mesh.GetMaterial();
		TSharedPtr<FPipeline> Pipeline = Material.GetPipeline();
		Pipeline->Bind();

		UpdateMaterialUniforms(Material);
		UpdateMatrixUniforms(Pipeline, InModel, Camera);
		UpdateCameraUniforms(Pipeline, Camera);
		UpdateLightUniforms(Pipeline, Scene);

		Mesh.GetVertexArray()->Bind();

		int32 NumIndices = Mesh.GetVertexArray()->GetNumIndices();
		if (NumIndices > 0)
		{
			FRHI::DrawIndexed(NumIndices);
		}
		else
		{
			FRHI::Draw(Mesh.GetVertexArray()->GetNumVertices());
		}
	}
}

// Returns a string in the form of "Material.PropertyName"
static std::string GetMaterialUniformMemberName(const ANSICHAR* InPropertyName)
{
	return std::string(FUniformNames::Material) + "." + InPropertyName;
}

// Returns a string in the form of "UniformName[LightIndex].MemberName" (e.g. "PointLight[0].Position").
static std::string GetLightUniformMemberName(const ANSICHAR* InUniformName, int32 LightIndex, const ANSICHAR* InMemberName)
{
	return std::string(InUniformName) + "[" + std::to_string(LightIndex) + "]." + InMemberName;
}

static void UpdateMaterialUniforms(FMaterial& InMaterial)
{
	TSharedPtr<FPipeline> Pipeline = InMaterial.GetPipeline();

	for (const FMaterialProperty<float>& FloatProperty : InMaterial.GetFloatProperties())
	{
		std::string FloatPropertyName = GetMaterialUniformMemberName(FloatProperty.Name.GetString().GetData());
		Pipeline->SetFloat(FloatPropertyName.c_str(), FloatProperty.Property);
	}

	for (const FMaterialProperty<FColor>& ColorProperty : InMaterial.GetColorProperties())
	{
		std::string ColorPropertyName = GetMaterialUniformMemberName(ColorProperty.Name.GetString().GetData());
		Pipeline->SetVector3D(ColorPropertyName.c_str(), ColorProperty.Property);
	}

	for (const FMaterialProperty<FTexture2D>& TextureProperty : InMaterial.GetTextureProperties())
	{
		TextureProperty.Property.Bind();
		std::string TexturePropertyName = GetMaterialUniformMemberName(TextureProperty.Name.GetString().GetData());
		int32 TextureUnit = static_cast<int32>(TextureProperty.Property.GetTextureUnit());
		Pipeline->SetInt(TexturePropertyName.c_str(), TextureUnit);
	}
}

static void UpdateMatrixUniforms(const TSharedPtr<FPipeline>& InPipeline, const TSharedPtr<FModel>& InModel, const TSharedPtr<FCamera>& InCamera)
{
	InPipeline->SetMatrix4D(FUniformNames::ModelMatrix, InModel->GetWorldTransform().ToMatrix());
	InPipeline->SetMatrix4D(FUniformNames::ViewMatrix, InCamera->GetLookAt().ToMatrix());
	InPipeline->SetMatrix4D(FUniformNames::ProjectionMatrix, InCamera->GetProjection());
}

static void UpdateCameraUniforms(const TSharedPtr<FPipeline>& InPipeline, const TSharedPtr<FCamera>& InCamera)
{
	InPipeline->SetVector3D(FUniformNames::CameraPosition, InCamera->GetPosition());
}

static void UpdateLightUniforms(const TSharedPtr<FPipeline>& InPipeline, const TSharedPtr<FScene>& InScene)
{
	const TArray<TSharedPtr<FDirectionalLight> >& DirectionalLights = InScene->GetVisibleDirectionalLights();
	for (int32 Index = 0; Index < DirectionalLights.GetSize(); ++Index)
	{
		std::string Direction = GetLightUniformMemberName(FUniformNames::DirectionalLights, Index, "Direction");
		std::string Color = GetLightUniformMemberName(FUniformNames::DirectionalLights, Index, "Color");
		std::string Intensity = GetLightUniformMemberName(FUniformNames::DirectionalLights, Index, "Intensity");

		InPipeline->SetVector3D(Direction.c_str(), DirectionalLights[Index]->GetDirection());
		InPipeline->SetVector3D(Color.c_str(), (FVector3D)DirectionalLights[Index]->GetColor());
		InPipeline->SetFloat(Intensity.c_str(), DirectionalLights[Index]->GetIntensity());
	}

	InPipeline->SetInt(FUniformNames::NumDirectionalLights, DirectionalLights.GetSize());

	const TArray<TSharedPtr<FPointLight> >& PointLights = InScene->GetVisiblePointLights();
	for (int32 Index = 0; Index < PointLights.GetSize(); ++Index)
	{
		std::string Position = GetLightUniformMemberName(FUniformNames::PointLights, Index, "Position");
		std::string Color = GetLightUniformMemberName(FUniformNames::PointLights, Index, "Color");
		std::string ConstantAttenuationFactor = GetLightUniformMemberName(FUniformNames::PointLights, Index, "Attenuation.Constant");
		std::string LinearAttenuationFactor = GetLightUniformMemberName(FUniformNames::PointLights, Index, "Attenuation.Linear");
		std::string QuadraticAttenuationFactor = GetLightUniformMemberName(FUniformNames::PointLights, Index, "Attenuation.Quadratic");
		std::string Intensity = GetLightUniformMemberName(FUniformNames::PointLights, Index, "Intensity");

		InPipeline->SetVector3D(Position.c_str(), PointLights[Index]->GetPosition());
		InPipeline->SetVector3D(Color.c_str(), (FVector3D)PointLights[Index]->GetColor());
		InPipeline->SetFloat(ConstantAttenuationFactor.c_str(), PointLights[Index]->GetAttenuation().Constant);
		InPipeline->SetFloat(LinearAttenuationFactor.c_str(), PointLights[Index]->GetAttenuation().Linear);
		InPipeline->SetFloat(QuadraticAttenuationFactor.c_str(), PointLights[Index]->GetAttenuation().Quadratic);
		InPipeline->SetFloat(Intensity.c_str(), PointLights[Index]->GetIntensity());
	}

	InPipeline->SetInt(FUniformNames::NumPointLights, PointLights.GetSize());
}
