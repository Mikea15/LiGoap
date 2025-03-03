// Michael Adaixo - 2025

#pragma once

#include <sstream>

#include "types.h"

struct Symbol
{
	union
	{
		float fValue;
		int iValue;
		bool bValue;
		void* ptrValue = nullptr;
	};
};

struct WorldState
{
	// Bit Position; KeyAtom
	// Bit Value; Bool
	uint64_t stateBits;

	bool operator==(const WorldState& other) const { return stateBits == other.stateBits; }
};

inline void WorldStateInit(WorldState& state)
{
	state.stateBits = 0;
}

inline void WorldStateSetBit(WorldState& state, EKeyAtom key)
{
	state.stateBits |= (1ull << static_cast<uint64_t>(key));
}

inline void WorldStateClearBit(WorldState& state, EKeyAtom key)
{
	state.stateBits &= ~(1ull << static_cast<uint64_t>(key));
}

inline void WorldStateWriteBit(WorldState& state, EKeyAtom key, bool value)
{
	if (value)
	{
		WorldStateSetBit(state, key);
	}
	else
	{
		WorldStateClearBit(state, key);
	}
}

inline bool WorldStateReadBit(const WorldState& state, EKeyAtom key)
{
	return (state.stateBits & (1ull << static_cast<uint64_t>(key))) != 0;
}

inline void WorldStatePrint(const WorldState& state)
{
	std::cout << "World State: \n";
	std::cout << "64th Key |--- ---| 0th Key\n";
	
	// Print the bits from most significant to least significant
	for (int i = 63; i >= 0; i--) {
		printf("%d", (state.stateBits & (1ULL << i)) ? 1 : 0);
        
		// Add a space every 8 bits for readability
		if (i % 8 == 0 && i > 0) {
			printf(" ");
		}
	}
	printf("\n");
}

template<>
struct std::hash<WorldState>
{
	size_t operator()(const WorldState& state) const noexcept
	{
		return state.stateBits;
	}
};
