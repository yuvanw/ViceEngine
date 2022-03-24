#pragma once

enum class EMemoryAlignment : uint8
{
	One = 1,
	Two = 1 << 1,
	Four = 1 << 2,
	Eight = 1 << 3,
	Default = Eight
};
