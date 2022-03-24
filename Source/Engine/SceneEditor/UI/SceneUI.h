#pragma once

#include "CoreMinimal.h"

/**
 * The scene UI draws the current scene inside a window and draws debug information
 * such as the current application frame rate. It also keeps track of whether the mouse
 * is covered hovered over the scene.
 */
class FSceneUI
{
public:
	FSceneUI() = default;
	~FSceneUI() = default;

	// Non-copyable.
	FSceneUI(const FSceneUI&) = default;
	FSceneUI& operator=(const FSceneUI&) = default;

	// Non-movable.
	FSceneUI(FSceneUI&&) = default;
	FSceneUI& operator=(FSceneUI&&) = default;

	void Init();
	void Shutdown();
	void Update();

	bool IsHovered() const;

	static const ANSICHAR* WindowName;

private:
	// Data used to calculate frames per second.
	double PreviousTimeMilliseconds;
	double CurrentTimeMilliseconds;

	// Whether or not the mouse is hovered over the scene window.
	bool bIsHovered;

	void RenderScene();
	void RenderDebugInfo();
};
