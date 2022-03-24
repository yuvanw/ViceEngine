#pragma once

struct ImDrawData;

class FImGuiRHI
{
public:
	static bool Init();
	static void Shutdown();
	static void NewFrame();
	static void RenderDrawData(ImDrawData* InDrawData);
};
