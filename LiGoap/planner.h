// Michael Adaixo - 2025

#pragma once

#include <queue>
#include <unordered_set>

#include "action.h"
#include "goal.h"
#include "binary_heap.h"

inline void PlanPrint(const std::vector<Action*>& plan)
{
	printf("Plan: \n");
	for (const Action* action : plan)
	{
		printf(" -> %s", action->name.c_str());
	}
	printf("\n");
}

inline std::vector<Action*> Plan(const Goal& goal, const WorldState& state, const std::vector<Action*>& actions)
{
	struct PlanNode
	{
		std::vector<Action*> plan;
		WorldState state;
		float g; // current plan
		float h; // heuristic
		float f; // total

		bool operator<(const PlanNode& Other) const { return f < Other.f; }
		bool operator>(const PlanNode& Other) const { return f > Other.f; }
	};

	const int reserveSpace = 100;
	BinaryHeap<PlanNode> openList(reserveSpace);

	std::unordered_set<WorldState> closedList; // Visited States

	// Start node
	openList.insert({ {}, state, 0, 0, GoalDistanceToState(goal, state) });

	// Reuse these vectors to avoid allocations in the loop
	std::vector<Action*> newPlan;
	newPlan.reserve(15);

	while (!openList.empty())
	{
		PlanNode current = openList.extractMin();

		// Check if goal reached
		if (GoalDistanceToState(goal, current.state) <= 0)
		{
			return current.plan;
		}

		// Add current state to closed list
		WorldState stateKey = current.state;
		if (closedList.find(stateKey) != closedList.end())
		{
			continue;
		}
		closedList.insert(stateKey);

		// Try each action
		for (size_t i = 0; i < actions.size(); ++i)
		{
			Action* action = actions[i];
			if (ActionMeetsPreconditions(*action, current.state))
			{
				// Create new state
				WorldState newState = current.state;

				// Apply Action effects to new state
				ActionApplyEffect(*action, newState);

				// Calculate Costs
				const float g = current.g + static_cast<float>(action->cost); // current + movement/action cost
				const float h = GoalDistanceToState(goal, newState);

				newPlan = current.plan;
				newPlan.push_back(action);

				openList.insert({ newPlan, newState, g, h, g + h });
			}
		}
	}

	return {}; // No plan found
}
