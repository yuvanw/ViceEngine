#include "SceneEditor.h"
#include "RenderManager.h"
#include "Scene/Scene.h"
#include "Camera/CameraController.h"
#include "RHI/RHI.h"

#include "UI/ImGui/ImGuiUtilities.h"
#include "UI/ImGui/HAL/PlatformImGui.h"
#include "UI/ImGui/RHI/ImGuiRHI.h"

// ImGui header containing docking API (e.g. DockBuilderGetNode, DockBuilderAddNode, etc.).
#include "imgui/imgui_internal.h"

void FSceneEditor::Init()
{
	// Create render target.
	FViewport Viewport = FRenderManager::GetRenderer()->GetViewport();
	TSharedPtr<FFrameBuffer> RenderTarget = MakeShared<FFrameBuffer>(Viewport.Width, Viewport.Height);
	FRenderManager::GetRenderer()->SetRenderTarget(RenderTarget);

	// Create default camera.
	FVector3D CameraPosition = { 5.0f, 5.0f, 5.0f };
	FVector3D CameraTarget = { 0.0f, 0.0f, 0.0f };
	TSharedPtr<FCamera> Camera = MakeShared<FCamera>(CameraPosition, CameraTarget);
	FRenderManager::GetRenderer()->SetCamera(Camera);
	CameraController = MakeUnique<FCameraController>(Camera);

	// Load default scene.
	FRenderManager::GetRenderer()->SetScene("Default.scn");

	// Initialize UI components.
	SceneUI.Init();
	OutlinerUI.Init();
	InspectorUI.Init();

	// Initialize Dear ImGui.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	SetDarkMode();

	ImGuiIO& io = ImGui::GetIO();
	// Enable keyboard controls.
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Enable ImGui window docking.
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Don't use imgui.ini file.
	io.IniFilename = nullptr;

	void* WindowHandle = FRenderManager::GetRenderer()->GetNativeWindowHandle();
	FPlatformImGui::Init(WindowHandle);
	FImGuiRHI::Init();
}

void FSceneEditor::Shutdown()
{
	SceneUI.Shutdown();
	OutlinerUI.Shutdown();
	InspectorUI.Shutdown();

	FImGuiRHI::Shutdown();
	FPlatformImGui::Shutdown();
	ImGui::DestroyContext();
}

void FSceneEditor::Update(float InDeltaTimeMilliseconds)
{
	CameraController->Update(InDeltaTimeMilliseconds);

	FImGuiRHI::NewFrame();
	FPlatformImGui::NewFrame();
	ImGui::NewFrame();

	BeginDockspace();

	SceneUI.Update();
	OutlinerUI.Update();
	InspectorUI.Update();
	UpdateSelectedSceneObject();

	EndDockspace();

	FRHI::EnableDepthTesting();
	FRHI::ClearColorBuffer();
	FRHI::ClearDepthBuffer();

	ImGui::Render();
	FImGuiRHI::RenderDrawData(ImGui::GetDrawData());
}

void FSceneEditor::OnKeyDown(const FKeyEvent& InKeyEvent)
{
	FPlatformImGui::OnKeyDown(InKeyEvent);
}

void FSceneEditor::OnKeyUp(const FKeyEvent& InKeyEvent)
{
	FPlatformImGui::OnKeyUp(InKeyEvent);
}

void FSceneEditor::OnMouseDown(const FMouseButtonEvent& InMouseButtonEvent)
{
	FPlatformImGui::OnMouseDown(InMouseButtonEvent);
	if (SceneUI.IsHovered())
	{
		CameraController->OnMouseDown(InMouseButtonEvent);
	}
}

void FSceneEditor::OnMouseUp(const FMouseButtonEvent& InMouseButtonEvent)
{
	FPlatformImGui::OnMouseUp(InMouseButtonEvent);
	if (SceneUI.IsHovered())
	{
		CameraController->OnMouseUp(InMouseButtonEvent);
	}
}

void FSceneEditor::OnMouseMove(const FMouseMoveEvent& InMouseMoveEvent)
{
	if (SceneUI.IsHovered())
	{
		CameraController->OnMouseMove(InMouseMoveEvent);
	}
}

void FSceneEditor::OnMouseScroll(const FMouseScrollEvent& InMouseScrollEvent)
{
	if (SceneUI.IsHovered())
	{
		CameraController->OnMouseScroll(InMouseScrollEvent);
	}
}

void FSceneEditor::UpdateSelectedSceneObject()
{
	if (TSharedPtr<FCamera> Camera = OutlinerUI.GetSelectedCamera())
	{
		InspectorUI.SetSelected(Camera);
	}
	else if (TSharedPtr<FSkybox> Skybox = OutlinerUI.GetSelectedSkybox())
	{
		InspectorUI.SetSelected(Skybox);
	}
	else if (TSharedPtr<FModel> Model = OutlinerUI.GetSelectedModel())
	{
		InspectorUI.SetSelected(Model);
	}
	else if (TSharedPtr<FDirectionalLight> DirectionalLight = OutlinerUI.GetSelectedDirectionalLight())
	{
		InspectorUI.SetSelected(DirectionalLight);
	}
	else if (TSharedPtr<FPointLight> PointLight = OutlinerUI.GetSelectedPointLight())
	{
		InspectorUI.SetSelected(PointLight);
	}
}

/* static */ void FSceneEditor::SetDarkMode()
{
	/**
	 * Color presets referenced from the following repository:
	 * https://github.com/jayanam/jgl_demos/blob/3d887e28ee8566ab79365505be2eeff628387d9e/JGL_MeshLoader/source/render/ui_context.cpp#L28
	 */
	auto& Colors = ImGui::GetStyle().Colors;
	Colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };

	Colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
	Colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
	Colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

	Colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
	Colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
	Colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

	Colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
	Colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
	Colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

	Colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
	Colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.38f, 0.38f, 1.0f };
	Colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.28f, 0.28f, 1.0f };
	Colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
	Colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

	Colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
	Colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
	Colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
}

/* static */ void FSceneEditor::BeginDockspace()
{
	/**
	 * Create default docked window layout.
	 * Dockspace setup code referenced from: https://gitter.im/mosra/magnum/archives/2019/12/05
	 */
	ImGuiWindowFlags WindowFlags =
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground;

	ImGuiViewport* ImGuiViewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImGuiViewport->Pos);
	ImGui::SetNextWindowSize(ImGuiViewport->Size);
	ImGui::SetNextWindowViewport(ImGuiViewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("InvisibleWindow", nullptr, WindowFlags);
	ImGui::PopStyleVar(3);

	ImGuiID DockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

	// Setup docked window layout if we haven't already.
	if (!ImGui::DockBuilderGetNode(DockSpaceId)) {
		ImGui::DockBuilderAddNode(DockSpaceId, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(DockSpaceId, ImGuiViewport->Size);

		ImGuiID DockSceneId = DockSpaceId;
		// Dock the Outliner window to the left side of the window and have it take up 25% of the total window space.
		ImGuiID DockOutlinerId = ImGui::DockBuilderSplitNode(DockSceneId, ImGuiDir_Left, 0.25f, nullptr, &DockSceneId);
		// Dock the Inspector window to the bottom of the Outliner window and have it take up 50% of the Outliner window space.
		ImGuiID DockInspectorId = ImGui::DockBuilderSplitNode(DockOutlinerId, ImGuiDir_Down, 0.6f, nullptr, &DockOutlinerId);

		ImGui::DockBuilderDockWindow(FSceneUI::WindowName, DockSceneId);
		ImGui::DockBuilderDockWindow(FOutlinerUI::WindowName, DockOutlinerId);
		ImGui::DockBuilderDockWindow(FInspectorUI::WindowName, DockInspectorId);

		ImGui::DockBuilderFinish(DockSceneId);
	}

	ImGui::DockSpace(DockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
}

/* static */ void FSceneEditor::EndDockspace()
{
	ImGui::End();
}
