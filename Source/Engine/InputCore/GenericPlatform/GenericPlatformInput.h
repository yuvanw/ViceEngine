#pragma once

#include "CoreMinimal.h"
#include "Keys.h"

class FGenericPlatformInput
{
public:
	// Empty default implementation.
	static FKey GetKeyFromVirtualKeyCode(int32 InVirtualKeyCode) 
	{ 
		return nullptr; 
	}
};
