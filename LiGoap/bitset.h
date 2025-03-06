// Michael Adaixo - 2025

#pragma once

#include <cstdint>
#include <cstdio>

#include "types.h"

using bitset64_t = uint64_t;

inline void BitsetInit(bitset64_t& bitset)
{
	bitset = 0;
}

inline void BitsetSet(bitset64_t& bitset, EKeyAtom key)
{
	bitset |= (1ull << static_cast<uint64_t>(key));
}

inline void BitsetClear(bitset64_t& bitset, EKeyAtom key)
{
	bitset &= ~(1ull << static_cast<uint64_t>(key));
}

inline void BitsetCombine(bitset64_t& inOutBitset, bitset64_t bitset)
{
	inOutBitset |= bitset;
}

inline void BitsetWrite(bitset64_t& bitset, EKeyAtom key, bool value)
{
	if (value)
	{
		BitsetSet(bitset, key);
	}
	else
	{
		BitsetClear(bitset, key);
	}
}

inline bool BitsetRead(bitset64_t bitset, EKeyAtom key)
{
	return (bitset & (1ull << static_cast<uint64_t>(key))) != 0;
}

inline void BitsetPrint(bitset64_t bitset)
{
	printf("World State: \n");
	printf("64th Key |--- ---| 0th Key\n");
	
	// Print the bits from most significant to the least significant
	for (int i = 63; i >= 0; i--) {
		printf("%d", (bitset & (1ULL << i)) ? 1 : 0);
		if (i % 8 == 0 && i > 0) {
			printf(" ");
		}
	}
	printf("\n");
}
