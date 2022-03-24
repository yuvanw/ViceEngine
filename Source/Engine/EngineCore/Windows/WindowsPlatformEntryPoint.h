#pragma once

#include "CoreMinimal.h"
#include "Engine.h"

// System include for WinMain.
#include "Windows.h"

int32 WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	FEngine::Run();
	return 0;
}
