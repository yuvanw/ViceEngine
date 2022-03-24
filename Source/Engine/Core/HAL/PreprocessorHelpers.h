#pragma once

// Follows Unreal's implementation.
// Converts a preprocessor token into a string.
#define TO_STRING(Token) TO_STRING_IMPL(Token)
#define TO_STRING_IMPL(Token) #Token

// Follows Unreal's implementation.
// Concatenates two preprocessor tokens.
#define CONCAT(Token1, Token2) CONCAT_IMPL(Token1, Token2)
#define CONCAT_IMPL(Token1, Token2) Token1##Token2

/**
 * Follows Unreal's implementation.
 * 
 * Creates a string that can be used to include a platform-specific header in a platform-agnostic way.
 * For example, PLATFORM_HEADER(PlatformApplication.h) will expand to "Windows/WindowsPlatformApplication.h"
 * if the value of the PLATFORM_NAME preprocessor definition is "Windows", or "Mac/MacPlatformApplication.h" 
 * if its value is "Mac".
 */
#define PLATFORM_HEADER(Suffix) TO_STRING(CONCAT(PLATFORM_NAME/PLATFORM_NAME, Suffix))
