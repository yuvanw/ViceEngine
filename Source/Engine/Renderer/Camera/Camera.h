#pragma once

#include "CoreMinimal.h"
#include "Frustum.h"

enum class EProjectionType
{
	Perspective,
	Orthographic
};

/**
 * The camera specifies a view through which to render a scene.
 * This view is defined by the camera's position and the target
 * it is looking at. Additionally, the camera's projection type
 * specifies whether to render using a perspective or orthographic
 * projection.
 */
class FCamera
{
public:
	FCamera();
	explicit FCamera(
		const FVector3D& InPosition, 
		const FVector3D& InTarget, 
		EProjectionType ProjectionType = EProjectionType::Perspective
	);
	~FCamera() = default;

	FCamera(const FCamera&) = default;
	FCamera& operator=(const FCamera&) = default;
	FCamera(FCamera&&) = default;
	FCamera& operator=(FCamera&&) = default;

	// Getters.
	const FVector3D& GetPosition() const 
	{ 
		return Position; 
	};
	const FVector3D& GetTarget() const 
	{
		return Target; 
	};

	const FVector3D& GetForward() const 
	{ 
		return Forward; 
	};
	const FVector3D& GetRight() const 
	{ 
		return Right; 
	};
	const FVector3D& GetUp() const 
	{ 
		return Up;
	};
	
	EProjectionType GetProjectionType() const 
	{ 
		return ProjectionType; 
	}
	
	const FFrustum& GetFrustum() const 
	{ 
		return Frustum; 
	}
	float GetNearPlaneDistance() const
	{ 
		return Frustum.NearDistance; 
	}
	float GetFarPlaneDistance() const
	{ 
		return Frustum.FarDistance; 
	}

	const FTransform4D& GetLookAt() const 
	{ 
		return LookAt; 
	};
	const FMatrix4D& GetProjection() const 
	{ 
		return Projection; 
	};

	// Setters.
	void SetPosition(const FVector3D& InPosition);
	void SetTarget(const FVector3D& InTarget);

	void SetProjectionType(EProjectionType InProjectionType);
	
	void SetFrustum(const FFrustum& InFrustum);
	void SetNearPlaneDistance(float InDistance);
	void SetFarPlaneDistance(float InDistance);
	void SetFrustumWidth(float InWidth);
	void SetFrustumHeight(float InHeight);

	static FTransform4D MakeLookAt(const FVector3D& InCameraPosition, const FVector3D& InTargetPosition);
	static FMatrix4D MakePerspectiveProjection(const FFrustum& InFrustum);
	static FMatrix4D MakeOrthographicProjection(const FFrustum& InFrustum);

private:
	void UpdateCameraOrientation();
	FMatrix4D MakeProjection();

	// Camera position.
	FVector3D Position;
	// Point that camera is looking at.
	FVector3D Target;

	// Camera axes.
	FVector3D Forward;
	FVector3D Right;
	FVector3D Up;

	// Projection type (perspective or orthographic).
	EProjectionType ProjectionType;

	// Camera frustum
	FFrustum Frustum;

	// Camera matrices.
	FTransform4D LookAt;
	FMatrix4D Projection;
};
