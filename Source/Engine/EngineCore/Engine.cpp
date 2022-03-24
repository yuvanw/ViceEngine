#include "Engine.h"

#include "CoreMinimal.h"
#include "HAL/PlatformApplication.h"
#include "HAL/PlatformTime.h"
#include "RenderManager.h"
#include "ViceApplication.h"
#include "SceneEditor.h"

static constexpr const ANSICHAR* WindowTitle = "Vice Engine";
static constexpr int32 DefaultWindowWidth = 1280;
static constexpr int32 DefaultWindowHeight = 720;

// Run engine at a fixed 60 frames per second.
static constexpr int32 FrameInterval = 16.6666;

void FEngine::Run()
{
	TUniquePtr<FGenericApplication> Application = FPlatformApplication::CreateApplication();
	TUniquePtr<FGenericWindow> Window = Application->MakeWindow(WindowTitle, DefaultWindowWidth, DefaultWindowHeight);
	Window->Show();
	
	// Initialize the renderer using the default window. 
	void* WindowHandle = Window->GetNativeWindowHandle();
	FViewport Viewport = { 0, 0, Window->GetWidth(), Window->GetHeight() };
	FRenderManager::Init(WindowHandle, Viewport);

	// Create the scene editor and subscribe it to the application message pump.
	TUniquePtr<FSceneEditor> SceneEditor = MakeUnique<FSceneEditor>();
	TSharedPtr<FViceApplication> ViceApplication = MakeShared<FViceApplication>(MoveTemp(SceneEditor));
	Application->SetApplicationMessageHandler(ViceApplication);
	ViceApplication->Init();

	FPlatformTime::Init();
	double PrevTime = FPlatformTime::CurrentTimeMilliseconds();
	double CurrTime = FPlatformTime::CurrentTimeMilliseconds();
	double DeltaTimeMilliseconds = 0.0;

	// Run engine loop.
	while (Application->IsRunning())
	{
		DeltaTimeMilliseconds += (CurrTime - PrevTime);
		if (DeltaTimeMilliseconds >= FrameInterval)
		{
			// Update engine subsystems.
			ViceApplication->Update(DeltaTimeMilliseconds);
			FRenderManager::Update(DeltaTimeMilliseconds);
			FPlatformApplication::PumpMessages();

			DeltaTimeMilliseconds -= FrameInterval;
		}

		PrevTime = CurrTime;
		CurrTime = FPlatformTime::CurrentTimeMilliseconds();
	}

	// Shutdown engine subsystems.
	ViceApplication->Shutdown();
	FRenderManager::Shutdown();
}
