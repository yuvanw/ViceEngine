#include "ImGuiRHI.h"

// Renderer include for GLSL version.
#include "RHI/RHIDefinitions.h"

#include "imgui/backends/imgui_impl_opengl3.h"

bool FImGuiRHI::Init()
{
	const ANSICHAR* GlslVersion = FShaderFiles::LanguageVersion;
	return ImGui_ImplOpenGL3_Init(GlslVersion);
}

void FImGuiRHI::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
}

void FImGuiRHI::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
}

void FImGuiRHI::RenderDrawData(ImDrawData* InDrawData)
{
	ImGui_ImplOpenGL3_RenderDrawData(InDrawData);
}
