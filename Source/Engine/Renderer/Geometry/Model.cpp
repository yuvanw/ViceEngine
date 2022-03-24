#include "Model.h"
#include "ModelFile.h"
#include "WavefrontObj.h"
#include "Scene/Scene.h"

FModel::FModel(const FStringId& InName, const FStringId& InModelFileName)
	: Name(InName)
	, Scene(nullptr)
	, DrawingMode(EDrawingMode::Filled)
	, Position(FVector3D::Zero)
	, Rotation(FVector3D::Zero)
	, Scale(FVector3D::One)
{
	FModelFile ModelFile = FModelFile(InModelFileName);
	const TArray<FStringId>& MeshFileNames = ModelFile.MeshFileNames;
	const TArray<FStringId>& MaterialFileNames = ModelFile.MaterialFileNames;
	for (int32 Index = 0; Index < MeshFileNames.GetSize(); ++Index)
	{
		Meshes.Emplace(MeshFileNames[Index], MaterialFileNames[Index]);
	}
}

FTransform4D FModel::GetWorldTransform()
{
	FTransform4D T = FTransform4D::MakeTranslation(Position);
	FTransform4D RX = FTransform4D::MakeRotationX(Rotation.X);
	FTransform4D RY = FTransform4D::MakeRotationY(Rotation.Y);
	FTransform4D RZ = FTransform4D::MakeRotationZ(Rotation.Z);
	FTransform4D S = FTransform4D::MakeScale(Scale);
	return T * RZ * RY * RX * S;
}

bool FModel::IsVisible() const
{
	if (Scene)
	{
		return Scene->IsModelVisible(Name);
	}

	return false;
}

void FModel::SetVisible()
{
	if (Scene)
	{
		Scene->SetModelVisible(Name);
	}
}

void FModel::SetInvisible()
{
	if (Scene)
	{
		Scene->SetModelInvisible(Name);
	}
}
