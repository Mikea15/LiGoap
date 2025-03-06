// Michael Adaixo - 2025

#pragma once

#include <iostream>

#include "bitset.h"
#include "worldstate.h"

// ACTION: Describes preconditions and effects
struct Action : NoCopy
{
	Action() = default;
	explicit Action(std::string&& inName, int inCost)
	{
		name = std::move(inName);
		cost = inCost;
	}
	// Allow move.
	Action(Action&& Move) = default;
	Action& operator=(Action&& Move) = default;

	key_map pre;
	key_map eff;
	std::string name = {};
	int cost = 0;
};

inline void ActionAddPrecondition(Action& action, const key_type& key, bool value)
{
	action.pre.insert({ key, value });
}

inline void ActionAddEffect(Action& action, const key_type& key, bool value)
{
	action.eff.insert({ key, value });
}

inline void ActionPrint(const Action& action)
{
	std::cout << "Name: [" << action.cost << "] " << action.name << "\n";
	// std::cout << "Preconditions: \n";
	// for (const auto& kvp : action.pre)
	// {
	// 	std::cout << "K: " << kvp.first << " - V: " << kvp.second << "\n";
	// }
	// std::cout << "Effects: \n";
	// for (const auto& kvp : action.eff)
	// {
	// 	std::cout << "K: " << kvp.first << " - V: " << kvp.second << "\n";
	// }
}

inline void ActionApplyEffect(const Action& action, WorldState& state)
{
	for (const auto& kvp : action.eff)
	{
		BitsetWrite(state.stateBits, kvp.first, kvp.second);
	}
}

inline bool ActionMeetsPreconditions(const Action& action, const WorldState& state)
{
	for (const auto& kvp : action.pre)
	{
		if (BitsetRead(state.stateBits, kvp.first) != kvp.second)
		{
			return false;
		}
	}
	return true;
}