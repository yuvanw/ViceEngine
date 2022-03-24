#include "SceneUI.h"
#include "RenderManager.h"
#include "HAL/PlatformTime.h"

#include "imgui/imgui.h"

const ANSICHAR* FSceneUI::WindowName = "Scene";

void FSceneUI::Init()
{
	PreviousTimeMilliseconds = 0.0f;
	CurrentTimeMilliseconds = 0.0f;
	bIsHovered = false;
}

void FSceneUI::Shutdown()
{
}

void FSceneUI::Update()
{
	RenderScene();
	RenderDebugInfo();
}

bool FSceneUI::IsHovered() const
{
	return bIsHovered;
}

void FSceneUI::RenderScene()
{
	ImGui::Begin(WindowName);

	// Keep track of whether the mouse is hovering over the scene window.
	bIsHovered = ImGui::IsWindowHovered();

	// Update Renderer viewport dimensions if the scene window has been resized.
	ImVec2 SceneWindowSize = ImGui::GetContentRegionAvail();
	FVector2D SceneWindowDimensions(SceneWindowSize.x, SceneWindowSize.y);
	FViewport Viewport = FRenderManager::GetRenderer()->GetViewport();
	FVector2D ViewportDimensions(Viewport.Width, Viewport.Height);
	if (SceneWindowDimensions != ViewportDimensions)
	{
		Viewport.Width = SceneWindowDimensions.X;
		Viewport.Height = SceneWindowDimensions.Y;
		FRenderManager::GetRenderer()->SetViewport(Viewport);
	}

	// Draw the output of the Renderer as an image in the Scene window.
	ImGui::Image(
		// Use the render target's texture as the image to draw to the Scene window.
		(void*)FRenderManager::GetRenderer()->GetRenderTarget()->GetTextureId(),
		SceneWindowSize,
		// Swap UV start and end coordinates so that the image doesn't appear upside down.
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	ImGui::End();
}

void FSceneUI::RenderDebugInfo()
{
	// Display debug info in bottom right corner of the window.
	static int Corner = 3;

	const float Padding = 20.0f;
	const ImGuiViewport* Viewport = ImGui::GetMainViewport();
	ImVec2 WorkPosition = Viewport->WorkPos;
	ImVec2 WorkSize = Viewport->WorkSize;
	ImVec2 WindowPosition, WindowPositionPivot;
	WindowPosition.x = (Corner & 1) ? (WorkPosition.x + WorkSize.x - Padding) : (WorkPosition.x + Padding);
	WindowPosition.y = (Corner & 2) ? (WorkPosition.y + WorkSize.y - Padding) : (WorkPosition.y + Padding);
	WindowPositionPivot.x = (Corner & 1) ? 1.0f : 0.0f;
	WindowPositionPivot.y = (Corner & 2) ? 1.0f : 0.0f;
	ImGui::SetNextWindowPos(WindowPosition, ImGuiCond_Always, WindowPositionPivot);
	ImGui::SetNextWindowViewport(Viewport->ID);

	// Transparent background.
	ImGui::SetNextWindowBgAlpha(0.35f);

	// Calculate frames per second.
	PreviousTimeMilliseconds = CurrentTimeMilliseconds;
	CurrentTimeMilliseconds = FPlatformTime::CurrentTimeMilliseconds();
	double DeltaTimeMilliseconds = CurrentTimeMilliseconds - PreviousTimeMilliseconds;
	double FramesPerSecond = 1000.0f / DeltaTimeMilliseconds;

	ImGui::Begin("Debug Info:", (bool*)true, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.1f", FramesPerSecond);

	TSharedPtr<FCamera> Camera = FRenderManager::GetRenderer()->GetCamera();
	ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)", Camera->GetPosition().X, Camera->GetPosition().Y, Camera->GetPosition().Z);

	ImGui::End();
}
