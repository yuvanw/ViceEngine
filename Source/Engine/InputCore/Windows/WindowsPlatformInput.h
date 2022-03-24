#pragma once

#include "GenericPlatform/GenericPlatformInput.h"
#include "Keys.h"

class FWindowsPlatformInput : public FGenericPlatformInput
{
public:
	static FKey GetKeyFromVirtualKeyCode(int32 InVirtualKeyCode);
};

using FPlatformInput = FWindowsPlatformInput;
