#include "Camera.h"

FCamera::FCamera()
	// The camera is at the world axes origin by default.
	: Position({ 0.0f, 0.0f, 0.0f})
	// The camera is looking down the Z-axis by default.
	, Target({ 0.0f, 0.0f, -1.0f })
	, Forward(FVector3D::Normalize(Target - Position))
	, Right(FVector3D::CrossProduct(Forward, FVector3D::YAxis))
	, Up(FVector3D::CrossProduct(Forward, Right))
	, ProjectionType(EProjectionType::Perspective)
	, Frustum()
	, LookAt(MakeLookAt(Position, Target))
	, Projection(MakeProjection())
{
}

FCamera::FCamera(
	const FVector3D& InPosition, 
	const FVector3D& InTarget, 
	EProjectionType ProjectionType /* = EProjectionType::Perspective */
)
	: Position(InPosition)
	, Target(InTarget)
	, Forward(FVector3D::Normalize(Target - Position))
	, Right(FVector3D::CrossProduct(Forward, FVector3D::YAxis))
	, Up(FVector3D::CrossProduct(Forward, Right))
	, ProjectionType(ProjectionType)
	, Frustum()
	, LookAt(MakeLookAt(Position, Target))
	, Projection(MakeProjection())
{
}

// Implementation for computing the perspective projection matrix taken from GLM.
/*static*/ FTransform4D FCamera::MakeLookAt(const FVector3D& InCameraPosition, const FVector3D& InTargetPosition)
{
	FVector3D ZAxis = InCameraPosition - InTargetPosition;
	// Using FVector3D::YAxis as the world space up direction.
	FVector3D XAxis = FVector3D::CrossProduct(FVector3D::YAxis, ZAxis);
	FVector3D YAxis = FVector3D::CrossProduct(ZAxis, XAxis);

	XAxis.Normalize();
	YAxis.Normalize();
	ZAxis.Normalize();

	// Inverse of a rotation matrix is its transpose.
	FTransform4D InverseRotation(
		XAxis.X, XAxis.Y, XAxis.Z, 0.0f,
		YAxis.X, YAxis.Y, YAxis.Z, 0.0f,
		ZAxis.X, ZAxis.Y, ZAxis.Z, 0.0f
	);

	// Inverse of a translation matrix has the translation column negated.
	FTransform4D InverseTranslation;
	InverseTranslation.SetTranslation(-InCameraPosition);

	return InverseRotation * InverseTranslation;
}

/**
* Perspective projection matrix computation taken from GLM.
* See: https://github.com/g-truc/glm/blob/master/glm/ext/matrix_clip_space.inl
*/
/*static*/ FMatrix4D FCamera::MakePerspectiveProjection(const FFrustum& InFrustum)
{
	const float TanHalfFov = FMath::Tan(InFrustum.VerticalFovRadians / 2.0f);
	const float AspectRatio = InFrustum.Width / InFrustum.Height;

	FMatrix4D Perspective;
	Perspective[0][0] = 1.0f / (AspectRatio * TanHalfFov);
	Perspective[1][1] = 1.0f / TanHalfFov;
	Perspective[2][2] = -(InFrustum.FarDistance + InFrustum.NearDistance) / (InFrustum.FarDistance - InFrustum.NearDistance);
	Perspective[2][3] = -1.0f;
	Perspective[3][2] = -(2.0f * InFrustum.FarDistance * InFrustum.NearDistance) / (InFrustum.FarDistance - InFrustum.NearDistance);

	return Perspective;
}

/**
* Orthographic projection matrix computation taken from GLM.
* See: https://github.com/g-truc/glm/blob/master/glm/ext/matrix_clip_space.inl
*/
/*static*/ FMatrix4D FCamera::MakeOrthographicProjection(const FFrustum& InFrustum)
{
	FMatrix4D Orthographic;
	
	float HalfWidth = InFrustum.Width / 2.0f;
	float HalfHeight = InFrustum.Height / 2.0f;
	float Right = HalfWidth / 100.0f;
	float Top = HalfHeight / 100.0f;
	
	Orthographic[0][0] = 1.0f / Right;
	Orthographic[1][1] = 1.0f / Top;
	Orthographic[2][2] = -2.0f / (InFrustum.FarDistance - InFrustum.NearDistance);
	Orthographic[3][2] = -(InFrustum.FarDistance + InFrustum.NearDistance) / (InFrustum.FarDistance - InFrustum.NearDistance);
	Orthographic[3][3] = 1.0f;

	return Orthographic;
}

void FCamera::SetPosition(const FVector3D& InPosition)
{
	Position = InPosition;
	UpdateCameraOrientation();
}

void FCamera::SetTarget(const FVector3D& InTarget)
{
	Target = InTarget;
	UpdateCameraOrientation();
}

void FCamera::SetProjectionType(EProjectionType InProjectionType)
{
	// Update projection matrix if we're switching to a different project type.
	if (ProjectionType != InProjectionType)
	{
		ProjectionType = InProjectionType;
		Projection = MakeProjection();
	}
}

void FCamera::SetFrustum(const FFrustum& InFrustum) 
{ 
	Frustum = InFrustum; 
	Projection = MakeProjection();
}

void FCamera::SetNearPlaneDistance(float InDistance) 
{ 
	Frustum.NearDistance = InDistance;
	Projection = MakeProjection();
}

void FCamera::SetFarPlaneDistance(float InDistance) 
{ 
	Frustum.FarDistance = InDistance;
	Projection = MakeProjection();
}

void FCamera::SetFrustumWidth(float InWidth)
{
	Frustum.Width = InWidth;
	Projection = MakeProjection();
}

void FCamera::SetFrustumHeight(float InHeight)
{
	Frustum.Height = InHeight;
	Projection = MakeProjection();
}

void FCamera::UpdateCameraOrientation()
{
	LookAt = MakeLookAt(Position, Target);
	Forward = FVector3D::Normalize(Target - Position);
	Right = FVector3D::CrossProduct(Forward, FVector3D::YAxis);
	Up = FVector3D::CrossProduct(Forward, Right);
}

FMatrix4D FCamera::MakeProjection()
{
	return (ProjectionType == EProjectionType::Perspective) ? MakePerspectiveProjection(Frustum) : MakeOrthographicProjection(Frustum);
}
