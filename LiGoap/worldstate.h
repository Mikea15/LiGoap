// Michael Adaixo - 2025

#pragma once

#include "types.h"

struct WorldState
{
	// Bit Position; KeyAtom
	// Bit Value; Bool
	uint64_t stateBits;

	bool operator==(const WorldState& other) const { return stateBits == other.stateBits; }
};

template<>
struct std::hash<WorldState>
{
	size_t operator()(const WorldState& state) const noexcept
	{
		return state.stateBits;
	}
};
