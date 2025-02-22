// Michael Adaixo - 2025

#include <chrono>
#include <iomanip>
#include <random>

#include "planner.h"

struct PlannerStats
{
	size_t total_plans;
	double plans_per_second;
};

PlannerStats BenchmarkPlanner(const Goal& goal, const WorldState& initial_state,
	const std::vector<Action>& actions,
	std::chrono::seconds duration = std::chrono::seconds(10))
{
	using clock = std::chrono::steady_clock;
	const auto start_time = clock::now();
	const auto end_time = start_time + duration;

	size_t plans_found = 0;
	std::vector<int> best_plan;
	float best_cost = std::numeric_limits<float>::max();

	// Keep planning until time runs out
	while (clock::now() < end_time)
	{
		// Create a copy of initial state for this iteration
		WorldState current_state = initial_state;

		// Add some randomization to help explore different paths
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> noise(-0.1, 0.1);

		// Modify action costs slightly to explore different paths
		std::vector<Action> temp_actions;
		temp_actions.reserve(actions.size());

		for (const auto& action : actions)
		{
			auto action_copy = action;
			action_copy.cost += noise(gen);
			temp_actions.push_back(action_copy);
		}

		// Try to find a plan
		auto plan = Plan(goal, current_state, temp_actions);
		if (!plan.empty())
		{
			plans_found++;
		}
	}

	// Calculate elapsed time and plans per second
	const auto elapsed = std::chrono::duration<double>(clock::now() - start_time).count();
	const double plans_per_second = static_cast<double>(plans_found) / elapsed;

	return PlannerStats{
		plans_found,
		plans_per_second
	};
}

void RunPlannerBenchmark(const Goal& goal, const WorldState& initial_state, const std::vector<Action>& actions)
{
	auto stats = BenchmarkPlanner(goal, initial_state, actions);

	std::cout << "Benchmark Results:\n"
		<< "Total plans found: " << stats.total_plans << "\n"
		<< "Plans per second: " << std::fixed << std::setprecision(2)
		<< stats.plans_per_second << "\n";
}

int main(int argc, char* argv[])
{
	Goal killEnemy = MakeGoal("KillEnemy");
	GoalAddSatisfaction(killEnemy, "kTargetIsDead", true);

	Action attack = MakeAction("Attack", 4);
	ActionAddPrecondition(attack, "kWeaponArmed", true);
	ActionAddPrecondition(attack, "kWeaponLoaded", true);
	ActionAddEffect(attack, "kTargetIsDead", true);

	Action attackFromCover = MakeAction("AttackFromCover", 1);
	ActionAddPrecondition(attackFromCover, "kHasCover", true);
	ActionAddPrecondition(attackFromCover, "kWeaponArmed", true);
	ActionAddPrecondition(attackFromCover, "kWeaponLoaded", true);
	ActionAddEffect(attackFromCover, "kTargetIsDead", true);

	Action attackFromVehicle = MakeAction("AttackFromVehicle", 1);
	ActionAddPrecondition(attackFromVehicle, "kAtObject", true);
	ActionAddPrecondition(attackFromVehicle, "kWeaponArmed", true);
	ActionAddPrecondition(attackFromVehicle, "kWeaponLoaded", true);
	ActionAddEffect(attackFromVehicle, "kTargetIsDead", true);

	Action goTo = MakeAction("GoTo", 2);
	ActionAddPrecondition(goTo, "kDestination", true);
	ActionAddEffect(goTo, "kAtObject", true);

	Action useObject = MakeAction("UseObject", 2);
	ActionAddPrecondition(useObject, "kAtObject", true);
	ActionAddEffect(useObject, "kHasCover", true); // flips object to the side to create cover.

	Action equipWeapon = MakeAction("EquipWeapon", 1);
	ActionAddEffect(equipWeapon, "kWeaponArmed", true);

	Action reloadWeapon = MakeAction("ReloadWeapon", 1);
	ActionAddPrecondition(reloadWeapon, "kWeaponArmed", true);
	ActionAddEffect(reloadWeapon, "kWeaponLoaded", true);

	// Basic movement and positioning actions
	Action sprint = MakeAction("Sprint", 3);
	ActionAddEffect(sprint, "kAtLocation", true);
	ActionAddEffect(sprint, "kStamina", false);

	Action walk = MakeAction("Walk", 5);
	ActionAddEffect(walk, "kAtLocation", true);

	Action crouch = MakeAction("Crouch", 1);
	ActionAddEffect(crouch, "kIsCrouched", true);
	ActionAddEffect(crouch, "kIsStealthy", true);

	Action prone = MakeAction("Prone", 2);
	ActionAddEffect(prone, "kIsProne", true);
	ActionAddEffect(prone, "kIsStealthy", true);
	ActionAddEffect(prone, "kAccuracyIncreased", true);

	// Cover and stealth actions
	Action findCover = MakeAction("FindCover", 2);
	ActionAddPrecondition(findCover, "kAtLocation", true);
	ActionAddEffect(findCover, "kHasCover", true);

	Action enterBuilding = MakeAction("EnterBuilding", 3);
	ActionAddPrecondition(enterBuilding, "kAtLocation", true);
	ActionAddEffect(enterBuilding, "kInsideBuilding", true);
	ActionAddEffect(enterBuilding, "kHasCover", true);

	Action climbVantagePoint = MakeAction("ClimbVantagePoint", 4);
	ActionAddPrecondition(climbVantagePoint, "kAtLocation", true);
	ActionAddPrecondition(climbVantagePoint, "kStamina", true);
	ActionAddEffect(climbVantagePoint, "kHasHighGround", true);
	ActionAddEffect(climbVantagePoint, "kAccuracyIncreased", true);

	// Weapon handling actions
	Action switchToSidearm = MakeAction("SwitchToSidearm", 1);
	ActionAddEffect(switchToSidearm, "kWeaponArmed", true);
	ActionAddEffect(switchToSidearm, "kUsingPistol", true);

	Action switchToRifle = MakeAction("SwitchToRifle", 2);
	ActionAddPrecondition(switchToRifle, "kHasRifle", true);
	ActionAddEffect(switchToRifle, "kWeaponArmed", true);
	ActionAddEffect(switchToRifle, "kUsingRifle", true);

	Action switchToSniper = MakeAction("SwitchToSniper", 2);
	ActionAddPrecondition(switchToSniper, "kHasSniper", true);
	ActionAddEffect(switchToSniper, "kWeaponArmed", true);
	ActionAddEffect(switchToSniper, "kUsingSniper", true);

	Action reloadPistol = MakeAction("ReloadPistol", 2);
	ActionAddPrecondition(reloadPistol, "kUsingPistol", true);
	ActionAddPrecondition(reloadPistol, "kHasPistolAmmo", true);
	ActionAddEffect(reloadPistol, "kWeaponLoaded", true);

	Action reloadRifle = MakeAction("ReloadRifle", 3);
	ActionAddPrecondition(reloadRifle, "kUsingRifle", true);
	ActionAddPrecondition(reloadRifle, "kHasRifleAmmo", true);
	ActionAddEffect(reloadRifle, "kWeaponLoaded", true);

	Action reloadSniper = MakeAction("ReloadSniper", 4);
	ActionAddPrecondition(reloadSniper, "kUsingSniper", true);
	ActionAddPrecondition(reloadSniper, "kHasSniperAmmo", true);
	ActionAddEffect(reloadSniper, "kWeaponLoaded", true);

	// Combat actions
	Action quickShot = MakeAction("QuickShot", 3);
	ActionAddPrecondition(quickShot, "kWeaponArmed", true);
	ActionAddPrecondition(quickShot, "kWeaponLoaded", true);
	ActionAddEffect(quickShot, "kTargetIsDead", true);
	ActionAddEffect(quickShot, "kWeaponLoaded", false);

	Action aimedShot = MakeAction("AimedShot", 5);
	ActionAddPrecondition(aimedShot, "kWeaponArmed", true);
	ActionAddPrecondition(aimedShot, "kWeaponLoaded", true);
	ActionAddPrecondition(aimedShot, "kAccuracyIncreased", true);
	ActionAddEffect(aimedShot, "kTargetIsDead", true);
	ActionAddEffect(aimedShot, "kWeaponLoaded", false);

	Action sniperShot = MakeAction("SniperShot", 6);
	ActionAddPrecondition(sniperShot, "kUsingSniper", true);
	ActionAddPrecondition(sniperShot, "kWeaponLoaded", true);
	ActionAddPrecondition(sniperShot, "kHasHighGround", true);
	ActionAddEffect(sniperShot, "kTargetIsDead", true);
	ActionAddEffect(sniperShot, "kWeaponLoaded", false);

	// Equipment actions
	Action searchForAmmo = MakeAction("SearchForAmmo", 4);
	ActionAddEffect(searchForAmmo, "kHasPistolAmmo", true);
	ActionAddEffect(searchForAmmo, "kHasRifleAmmo", true);

	Action findRifle = MakeAction("FindRifle", 3);
	ActionAddEffect(findRifle, "kHasRifle", true);

	Action findSniper = MakeAction("FindSniper", 5);
	ActionAddEffect(findSniper, "kHasSniper", true);

	Action useStim = MakeAction("UseStim", 2);
	ActionAddEffect(useStim, "kStamina", true);

	// Vehicle actions
	Action enterVehicle = MakeAction("EnterVehicle", 2);
	ActionAddPrecondition(enterVehicle, "kAtLocation", true);
	ActionAddEffect(enterVehicle, "kInVehicle", true);
	ActionAddEffect(enterVehicle, "kHasCover", true);

	Action startVehicle = MakeAction("StartVehicle", 1);
	ActionAddPrecondition(startVehicle, "kInVehicle", true);
	ActionAddEffect(startVehicle, "kVehicleRunning", true);

	Action driveVehicle = MakeAction("DriveVehicle", 3);
	ActionAddPrecondition(driveVehicle, "kInVehicle", true);
	ActionAddPrecondition(driveVehicle, "kVehicleRunning", true);
	ActionAddEffect(driveVehicle, "kAtLocation", true);

	// Support actions
	Action callSupport = MakeAction("CallSupport", 4);
	ActionAddPrecondition(callSupport, "kHasRadio", true);
	ActionAddEffect(callSupport, "kSupportAvailable", true);

	Action markTarget = MakeAction("MarkTarget", 2);
	ActionAddPrecondition(markTarget, "kHasHighGround", true);
	ActionAddEffect(markTarget, "kTargetMarked", true);

	Action callAirstrike = MakeAction("CallAirstrike", 8);
	ActionAddPrecondition(callAirstrike, "kSupportAvailable", true);
	ActionAddPrecondition(callAirstrike, "kTargetMarked", true);
	ActionAddEffect(callAirstrike, "kTargetIsDead", true);

	// Put all actions in the vector
	std::vector<Action> actions = {
		sprint,
		walk,
		crouch,
		prone,
		findCover,
		enterBuilding,
		climbVantagePoint,
		switchToSidearm,
		switchToRifle,
		switchToSniper,
		reloadPistol,
		reloadRifle,
		reloadSniper,
		quickShot,
		aimedShot,
		sniperShot,
		searchForAmmo,
		findRifle,
		findSniper,
		useStim,
		enterVehicle,
		startVehicle,
		driveVehicle,
		callSupport,
		markTarget,
		callAirstrike,
		// me
		attackFromCover,
		attack,
		attackFromVehicle,
		goTo,
		useObject,
		equipWeapon,
		reloadWeapon
	};

	WorldState currentState;
	currentState.db["kTargetIsDead"] = false;
	currentState.db["kWeaponArmed"] = false;
	currentState.db["kWeaponLoaded"] = false;
	currentState.db["kAtLocation"] = false;
	currentState.db["kStamina"] = true;
	currentState.db["kHasRadio"] = true;
	currentState.db["kHasPistolAmmo"] = true;
	currentState.db["kHasRifleAmmo"] = false;
	currentState.db["kHasSniperAmmo"] = false;
	currentState.db["kHasRifle"] = false;
	currentState.db["kHasSniper"] = false;
	currentState.db["kAccuracyIncreased"] = false;
	currentState.db["kHasHighGround"] = false;
	currentState.db["kHasCover"] = false;
	currentState.db["kIsStealthy"] = false;

	RunPlannerBenchmark(killEnemy, currentState, actions);

	WorldStatePrint(currentState);
	auto plan = Plan(killEnemy, currentState, actions);
	PlanPrint(plan);
	
	for (const Action& action : plan)
	{
		ActionApplyEffect(action, currentState);
	}
	WorldStatePrint(currentState);

	return 0;
}
