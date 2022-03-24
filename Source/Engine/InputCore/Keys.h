#pragma once

#include "CoreMinimal.h"

// Abstraction that represents any type of player input (keyboard key, mouse button, gamepad button, etc.)
class FKey
{
public:
	FKey(const ANSICHAR* InName)
		: KeyName(InName)
	{
	}
	~FKey() = default;

	const FStringId& GetKeyName() const 
	{ 
		return KeyName; 
	}

	bool operator==(const FKey& InKey) const
	{ 
		return KeyName == InKey.GetKeyName(); 
	}

private:
	FStringId KeyName;
};

// All currently supported FKeys.
struct EKeys
{
	// Mouse buttons.
	static const FKey LeftMouseButton;
	static const FKey RightMouseButton;
	static const FKey MiddleMouseButton;

	// English alphabet.
	static const FKey A;
	static const FKey B;
	static const FKey C;
	static const FKey D;
	static const FKey E;
	static const FKey F;
	static const FKey G;
	static const FKey H;
	static const FKey I;
	static const FKey J;
	static const FKey K;
	static const FKey L;
	static const FKey M;
	static const FKey N;
	static const FKey O;
	static const FKey P;
	static const FKey Q;
	static const FKey R;
	static const FKey S;
	static const FKey T;
	static const FKey U;
	static const FKey V;
	static const FKey W;
	static const FKey X;
	static const FKey Y;
	static const FKey Z;

	// Numbers.
	static const FKey Zero;
	static const FKey One;
	static const FKey Two;
	static const FKey Three;
	static const FKey Four;
	static const FKey Five;
	static const FKey Six;
	static const FKey Seven;
	static const FKey Eight;
	static const FKey Nine;

	// Arrow keys.
	static const FKey Left;
	static const FKey Up;
	static const FKey Right;
	static const FKey Down;

	// Function keys.
	static const FKey F1;
	static const FKey F2;
	static const FKey F3;
	static const FKey F4;
	static const FKey F5;
	static const FKey F6;
	static const FKey F7;
	static const FKey F8;
	static const FKey F9;
	static const FKey F10;
	static const FKey F11;
	static const FKey F12;
	
	// Modifier keys.
	static const FKey LeftShift;
	static const FKey LeftControl;
	static const FKey LeftAlt;
	static const FKey LeftCommand;
	static const FKey RightShift;
	static const FKey RightControl;
	static const FKey RightAlt;
	static const FKey RightCommand;

	// Misc.
	static const FKey Space;
	static const FKey Tab;
	static const FKey Delete;
	static const FKey Backspace;
	static const FKey Enter;

	static const FKey None;
};
