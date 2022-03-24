#pragma once

#include "WindowsPlatformInput.h"
#include <Windows.h>

FKey FWindowsPlatformInput::GetKeyFromVirtualKeyCode(int32 InVirtualKeyCode) 
{
	// Translate from Windows virtual key code to the corresponding FKey.
	switch (InVirtualKeyCode)
	{
		// Mouse buttons.
		case VK_LBUTTON: return EKeys::LeftMouseButton;
		case VK_RBUTTON: return EKeys::RightMouseButton;
		case VK_MBUTTON: return EKeys::MiddleMouseButton;

		// English alphabet.
		case 'A': return EKeys::A;
		case 'B': return EKeys::B;
		case 'C': return EKeys::C;
		case 'D': return EKeys::D;
		case 'E': return EKeys::E;
		case 'F': return EKeys::F;
		case 'G': return EKeys::G;
		case 'H': return EKeys::H;
		case 'I': return EKeys::I;
		case 'J': return EKeys::J;
		case 'K': return EKeys::K;
		case 'L': return EKeys::L;
		case 'M': return EKeys::M;
		case 'N': return EKeys::N;
		case 'O': return EKeys::O;
		case 'P': return EKeys::P;
		case 'Q': return EKeys::Q;
		case 'R': return EKeys::R;
		case 'S': return EKeys::S;
		case 'T': return EKeys::T;
		case 'U': return EKeys::U;
		case 'V': return EKeys::V;
		case 'W': return EKeys::W;
		case 'X': return EKeys::X;
		case 'Y': return EKeys::Y;
		case 'Z': return EKeys::Z;
		
		// Numbers.
		case '0': return EKeys::Zero;
		case '1': return EKeys::One;
		case '2': return EKeys::Two;
		case '3': return EKeys::Three;
		case '4': return EKeys::Four;
		case '5': return EKeys::Five;
		case '6': return EKeys::Six;
		case '7': return EKeys::Seven;
		case '8': return EKeys::Eight;
		case '9': return EKeys::Nine;

		// Arrow keys.
		case VK_LEFT:  return EKeys::Left;
		case VK_UP:	   return EKeys::Up;
		case VK_RIGHT: return EKeys::Right;
		case VK_DOWN:  return EKeys::Down;

		// Function keys.
		case VK_F1:	 return EKeys::F1;
		case VK_F2:	 return EKeys::F2;
		case VK_F3:	 return EKeys::F3;
		case VK_F4:	 return EKeys::F4;
		case VK_F5:	 return EKeys::F5;
		case VK_F6:	 return EKeys::F6;
		case VK_F7:	 return EKeys::F7;
		case VK_F8:	 return EKeys::F8;
		case VK_F9:	 return EKeys::F9;
		case VK_F10: return EKeys::F10;
		case VK_F11: return EKeys::F11;
		case VK_F12: return EKeys::F12;

		// Modifier keys.
		case VK_LSHIFT:   return EKeys::LeftShift;
		case VK_LCONTROL: return EKeys::LeftControl;
		case VK_LMENU:    return EKeys::LeftAlt;
		case VK_LWIN:     return EKeys::LeftCommand;
		case VK_RSHIFT:   return EKeys::RightShift;
		case VK_RCONTROL: return EKeys::RightControl;
		case VK_RMENU:    return EKeys::RightAlt;
		case VK_RWIN:     return EKeys::RightCommand;

		// Misc.
		case VK_SPACE:  return EKeys::Space;
		case VK_TAB:    return EKeys::Tab;
		case VK_DELETE: return EKeys::Delete;
		case VK_BACK:   return EKeys::Backspace;
		case VK_RETURN: return EKeys::Enter;

		default: return EKeys::None;
	}
}
