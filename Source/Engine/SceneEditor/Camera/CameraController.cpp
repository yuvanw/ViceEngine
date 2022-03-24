#include "CameraController.h"
#include "RenderManager.h"

// Limit how close the camera can be to its target.
static constexpr float MaxCameraToTargetLength = 2.0f;

static constexpr float TranslationSpeed = 0.0005f;
static constexpr float ZoomSpeed = 0.0005f;

FCameraController::FCameraController(const TSharedPtr<FCamera>& InCamera)
	: Camera(InCamera)
	, PressedButton(EKeys::None)
	, PreviousMousePosition(FVector2D::Zero)
	, CurrentMousePosition(FVector2D::Zero)
	, ScrollDelta(0.0f)
{
}

void FCameraController::Update(float InDeltaTimeMilliseconds)
{
	if (PressedButton == EKeys::LeftMouseButton)
	{
		Rotate();
	}
	else if (PressedButton == EKeys::RightMouseButton || PressedButton == EKeys::MiddleMouseButton)
	{
		Translate(InDeltaTimeMilliseconds);
	}

	if (!FMath::IsApproximatelyZero(ScrollDelta))
	{
		Zoom(InDeltaTimeMilliseconds);
	}

	PreviousMousePosition = CurrentMousePosition;
	ScrollDelta = 0.0f;
}

void FCameraController::OnMouseDown(const FMouseButtonEvent& InMouseButtonEvent)
{
	PressedButton = InMouseButtonEvent.GetKey();
}

void FCameraController::OnMouseUp(const FMouseButtonEvent& InMouseButtonEvent)
{
	FKey MouseButton = InMouseButtonEvent.GetKey();
	if (PressedButton == MouseButton)
	{
		PressedButton = EKeys::None;
	}
}

void FCameraController::OnMouseMove(const FMouseMoveEvent& InMouseMoveEvent)
{
	CurrentMousePosition = InMouseMoveEvent.GetMousePosition();
}

void FCameraController::OnMouseScroll(const FMouseScrollEvent& InMouseScrollEvent)
{
	ScrollDelta = InMouseScrollEvent.GetScrollDelta();
}

void FCameraController::Rotate()
{
	FVector2D MousePositionDelta = CurrentMousePosition - PreviousMousePosition;

	float WindowWidth = FRenderManager::GetRenderer()->GetViewport().Width;
	float WindowHeight = FRenderManager::GetRenderer()->GetViewport().Height;

	// Dragging the mouse across the entire width of the screen is a 360 degree rotation horizontally.
	float HorizontalRotation = FMath::TwoPi * (MousePositionDelta.X / WindowWidth);
	// Dragging the mouse across the entire height of the screen is a 180 degree rotation vertically.
	float VerticalRotation = FMath::Pi * (MousePositionDelta.Y / WindowHeight);

	// Rotate by negative horizontal and vertical rotation so that controls are inverted.
	FVector3D RotatedForward = FTransform4D::MakeRotation(-VerticalRotation, Camera->GetRight()) * FTransform4D::MakeRotationY(-HorizontalRotation) * Camera->GetForward();
	
	// Don't rotate if the camera is looking straight upward or downward.
	float ForwardDotUp = FVector3D::DotProduct(RotatedForward, FVector3D::YAxis);
	if (!FMath::IsApproximatelyZero(ForwardDotUp))
	{
		float Distance = FVector3D::Length(Camera->GetPosition(), Camera->GetTarget());
		FVector3D RotatedCameraPosition = Camera->GetTarget() + (-RotatedForward * Distance);
		Camera->SetPosition(RotatedCameraPosition);
	}
}

void FCameraController::Translate(float InDeltaTimeMilliseconds)
{
	FVector2D MousePositionDelta = CurrentMousePosition - PreviousMousePosition;

	float DistanceToOrigin = FVector3D::Length({ 0.0f, 0.0f, 0.0f }, Camera->GetPosition());
	DistanceToOrigin /= 10.0f;

	float TranslationDistance = TranslationSpeed * DistanceToOrigin * InDeltaTimeMilliseconds;
	FVector3D TranslationUp = Camera->GetUp() * MousePositionDelta.Y;
	FVector3D TranslationRight = Camera->GetRight() * MousePositionDelta.X;
	FVector3D Translation = (TranslationUp + TranslationRight) * TranslationDistance;

	// Subtract translation from camera position and target so that controls are inverted.
	Camera->SetPosition(Camera->GetPosition() - Translation);
	Camera->SetTarget(Camera->GetTarget() - Translation);
}

void FCameraController::Zoom(float InDeltaTimeMilliseconds)
{
	float ZoomDistance = ZoomSpeed * InDeltaTimeMilliseconds;
	FVector3D CameraPosition = Camera->GetPosition() + (Camera->GetForward() * ScrollDelta * ZoomDistance);
	float CameraToTargetLength = FVector3D::Length(CameraPosition, Camera->GetTarget());
	if (CameraToTargetLength >= MaxCameraToTargetLength)
	{
		Camera->SetPosition(CameraPosition);
	}
}
