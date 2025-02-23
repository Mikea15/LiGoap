// Michael Adaixo - 2025

#pragma once

#include <algorithm>
#include <unordered_set>

#include "action.h"
#include "goal.h"

inline void PlanPrint(const std::vector<Action>& plan)
{
	std::cout << "Plan: \n";
	for (const Action& action : plan)
	{
		std::cout<< " -> " << action.name;
	}
	std::cout << "\n";
}



std::vector<Action> Plan(const Goal& goal, const WorldState& state, const std::vector<Action>& actions)
{
	struct PlanNode
	{
		WorldState state;
		std::vector<Action> plan;
		float g; // current plan
		float h; // heuristic
		float f; // total
	};

	struct PlanNodeSort
	{
		bool operator()(const PlanNode& a, const PlanNode& b) const
		{
			return a.f > b.f;
		}
	};

	std::vector<PlanNode> openList;
	openList.reserve(150);
	
	std::unordered_set<std::string> closedList; // Track visited states

	// Start node
	openList.push_back({ state, {}, 0.0f, 0.0, GoalDistanceToState(goal, state) });

	// Reuse these vectors to avoid allocations in the loop
	std::vector<Action> newPlan;
	newPlan.reserve(15);

	while (!openList.empty())
	{
		// sort by f score.
		std::sort(openList.begin(), openList.end(), PlanNodeSort());

		PlanNode current = openList.back();
		openList.erase(openList.end() - 1);

		// Check if goal reached
		if (GoalDistanceToState(goal, current.state) <= 0.0f)
		{
			return current.plan;
		}

		// Add current state to closed list
		std::string stateKey = WorldStateToString(current.state);
		if (closedList.find(stateKey) != closedList.end())
		{
			continue;
		}
		closedList.insert(stateKey);

		// Try each action
		for (size_t i = 0; i < actions.size(); ++i)
		{
			const Action& action = actions[i];
			if (ActionMeetsPreconditions(action, current.state))
			{
				// Create new state
				WorldState newState = current.state;

				// Apply Action effects to new state
				ActionApplyEffect(action, newState);

				// Calculate Costs
				const float g =  current.g + static_cast<float>(action.cost); // current + movement/action cost
				const float h = GoalDistanceToState(goal, newState);

				newPlan = current.plan;
				newPlan.push_back(action);

				openList.push_back({ newState, newPlan, g, h, g + h });
			}
		}
	}

	return {}; // No plan found
}
