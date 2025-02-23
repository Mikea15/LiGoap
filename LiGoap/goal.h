// Michael Adaixo - 2025

#pragma once
#include <string>
#include <unordered_map>

#include "types.h"

// GOAL: Describes desired state
struct Goal
{
	Goal() = default;
	explicit Goal(const std::string& inName) { name = inName; }

	key_map satisfactions;
	std::string name = {};
};

inline void GoalAddSatisfaction(Goal& goal, const key_type& key, bool value)
{
	goal.satisfactions.insert(std::make_pair(key, value));
}

inline float GoalDistanceToState(const Goal& goal, const WorldState& state)
{
	float distance = 0.0f;
	for (const auto& kvp : goal.satisfactions)
	{
		auto it = state.db.find(kvp.first);
		if (it == state.db.end() || it->second != kvp.second)
		{
			distance += 1.0f;
		}
	}
	return distance;
}