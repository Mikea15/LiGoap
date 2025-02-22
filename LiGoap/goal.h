// Michael Adaixo - 2025

#pragma once
#include <string>
#include <unordered_map>

// GOAL: Describes desired state
struct Goal
{
	Goal() = default;
	explicit Goal(const std::string& inName) { name = inName; }

	std::unordered_map<std::string, bool> satisfactions;
	std::string name = {};
};

inline Goal MakeGoal(const std::string& name)
{
	return Goal(name);
}

inline void GoalAddSatisfaction(Goal& goal, const std::string& key, bool value)
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