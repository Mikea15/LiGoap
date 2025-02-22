// Michael Adaixo - 2025

#pragma once

#include <sstream>

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

using symbol_map = std::unordered_map<std::string, Symbol>;

struct WorldState
{
	std::unordered_map<std::string, bool> db;

	bool operator==(const WorldState& other) const
	{
		return db == other.db;
	}
};

inline std::string WorldStateToString(const WorldState& state)
{
	std::stringstream ss;
	for (const auto& kvp : state.db)
	{
		ss << kvp.first << ":" << kvp.second << ";";
	}
	return ss.str();
}

inline void WorldStatePrint(const WorldState& state)
{
	std::cout << "WorldState: \n" << WorldStateToString(state) << "\n";
}
