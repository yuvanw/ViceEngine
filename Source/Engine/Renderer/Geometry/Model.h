#pragma once

#include "CoreMinimal.h"
#include "Mesh.h"
#include "RHI/RHIDefinitions.h"

class FScene;

template <typename ObjectType>
class FSetScene;

/**
 * A model consists of a collection of meshes, as well as data that applies to 
 * all meshes, such as the transform (position, rotation, scale), drawing mode,
 * and visibility of the model.
 */
class FModel
{
public:
	explicit FModel(const FStringId& InName, const FStringId& InModelFileName);
	~FModel() = default;

	// Non-copyable.
	FModel(const FModel&) = delete;
	FModel& operator=(const FModel&) = delete;

	FModel(FModel&&) = default;
	FModel& operator=(FModel&&) = default;

	// Getters.
	const FStringId& GetName() const 
	{ 
		return Name; 
	}
	EDrawingMode GetDrawingMode() const 
	{ 
		return DrawingMode; 
	}
	TArray<FMesh>& GetMeshes() 
	{ 
		return Meshes;
	}
	const TArray<FMesh>& GetMeshes() const
	{
		return Meshes; 
	};
	const FVector3D& GetPosition() const
	{ 
		return Position;
	};
	const FVector3D& GetRotation() const 
	{
		return Rotation;
	};
	const FVector3D& GetScale() const 
	{
		return Scale;
	};
	FTransform4D GetWorldTransform();

	// Setters.
	void SetDrawingMode(EDrawingMode InDrawingMode) 
	{ 
		DrawingMode = InDrawingMode;
	}
	void SetPosition(const FVector3D& InPosition) 
	{ 
		Position = InPosition;
	}
	void SetRotation(const FVector3D& InRotation) 
	{ 
		Rotation = InRotation; 
	}
	void SetScale(const FVector3D& InScale) 
	{ 
		Scale = InScale; 
	}
	void SetVisible();
	void SetInvisible();
	
	bool IsVisible() const;

private:
	FStringId Name;
	FScene* Scene;

	TArray<FMesh> Meshes;
	
	EDrawingMode DrawingMode;
	
	// Transform.
	FVector3D Position;
	FVector3D Rotation;
	FVector3D Scale;
	
	friend class FSetScene<FModel>;
};
