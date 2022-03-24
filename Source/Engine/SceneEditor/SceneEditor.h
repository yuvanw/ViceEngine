#pragma once

#include "CoreMinimal.h"
#include "Game.h"
#include "UI/SceneUI.h"
#include "UI/OutlinerUI.h"
#include "UI/InspectorUI.h"

class FScene;
class FCameraController;

/**
 * The scene editor draws the current scene to a subsection of the application window.
 * It allows users modify and load scenes using the editor UI, as well as control the 
 * camera using a mouse.
 */
class FSceneEditor : public FGame
{
public:
	FSceneEditor() = default;
	~FSceneEditor() = default;

	// Non-copyable.
	FSceneEditor(const FSceneEditor&) = default;
	FSceneEditor& operator=(const FSceneEditor&) = default;

	// Non-movable.
	FSceneEditor(FSceneEditor&&) = default;
	FSceneEditor& operator=(FSceneEditor&&) = default;

	// Begin FGame interface.
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void Update(float InDeltaTimeMilliseconds) override;
	virtual void OnKeyDown(const FKeyEvent& InKeyEvent) override;
	virtual void OnKeyUp(const FKeyEvent& InKeyEvent) override;
	virtual void OnMouseDown(const FMouseButtonEvent& InMouseButtonEvent) override;
	virtual void OnMouseUp(const FMouseButtonEvent& InMouseButtonEvent) override;
	virtual void OnMouseMove(const FMouseMoveEvent& InMouseMoveEvent) override;
	virtual void OnMouseScroll(const FMouseScrollEvent& InMouseScrollEvent) override;
	// End FGame interface.

private:
	// Editor UI.
	FSceneUI SceneUI;
	FOutlinerUI OutlinerUI;
	FInspectorUI InspectorUI;

	// Mouse look camera controller.
	TUniquePtr<FCameraController> CameraController;

	void UpdateSelectedSceneObject();

	static void SetDarkMode();
	static void BeginDockspace();
	static void EndDockspace();
};
