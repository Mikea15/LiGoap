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
	key_map db;

	bool operator==(const WorldState& other) const {
		return db == other.db;
	}
};

inline std::string WorldStateToString(const WorldState& state)
{
	std::stringstream ss;
	for (const auto& kvp : state.db)
	{
		// ss << kvp.first << ":" << kvp.second << ";";
		ss << kvp.second << ";";
	}
	return ss.str();
}

inline void WorldStatePrint(const WorldState& state)
{
	std::cout << "WorldState: \n" << WorldStateToString(state) << "\n";
}
