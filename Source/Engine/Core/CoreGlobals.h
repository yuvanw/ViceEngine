#pragma once

// Global includes to support types.
#include <cstddef>
using int8 = char;
using uint8 = unsigned char;
using int16 = short;
using uint16 = unsigned short;
using int32 = int;
using uint32 = unsigned int;
using int64 = long long;
using uint64 = unsigned long long;

using U8CHAR = uint8;
using ANSICHAR = char;

// Invalid value for container index.
static constexpr int32 InvalidIndex = -1;
