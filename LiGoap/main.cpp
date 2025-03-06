// Michael Adaixo - 2025

#include <chrono>
#include <iomanip>
#include <random>

#include "planner.h"
#include "stringhash.h"

#define TEST_TIME 10

struct PlannerStats
{
	size_t total_plans;
	double plans_per_second;
};

PlannerStats BenchmarkPlanner(const Goal& goal, const WorldState& initial_state,
	const std::vector<Action*>& actions,
	std::chrono::seconds duration = std::chrono::seconds(TEST_TIME))
{
	using clock = std::chrono::high_resolution_clock;
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
		std::vector<Action*> temp_actions;
		temp_actions.reserve(actions.size());

		for (const auto& action : actions)
		{
			Action* newAction = new Action(std::string(action->name), action->cost + noise(gen));
			newAction->eff = action->eff;
			newAction->pre = action->pre;
			temp_actions.push_back(newAction);
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

void RunPlannerBenchmark(const Goal& goal, const WorldState& initial_state, const std::vector<Action*>& actions)
{
	auto stats = BenchmarkPlanner(goal, initial_state, actions);

	std::cout << "Benchmark Results:\n"
		<< "Total plans found: " << stats.total_plans << "\n"
		<< "Plans per second: " << std::fixed << std::setprecision(2)
		<< stats.plans_per_second << "\n";
}

int main(int argc, char* argv[])
{
	Goal killEnemy = Goal("KillEnemy");
	GoalAddSatisfaction(killEnemy, EKeyAtom::kTargetIsDead, true);

	Action attack = Action("Attack", 4);
	ActionAddPrecondition(attack, EKeyAtom::kWeaponArmed, true);
	ActionAddPrecondition(attack, EKeyAtom::kWeaponLoaded, true);
	ActionAddEffect(attack, EKeyAtom::kTargetIsDead, true);

	Action attackFromCover = Action("AttackFromCover", 1);
	ActionAddPrecondition(attackFromCover, EKeyAtom::kHasCover, true);
	ActionAddPrecondition(attackFromCover, EKeyAtom::kWeaponArmed, true);
	ActionAddPrecondition(attackFromCover, EKeyAtom::kWeaponLoaded, true);
	ActionAddEffect(attackFromCover, EKeyAtom::kTargetIsDead, true);

	Action attackFromVehicle = Action("AttackFromVehicle", 1);
	ActionAddPrecondition(attackFromVehicle, EKeyAtom::kAtObject, true);
	ActionAddPrecondition(attackFromVehicle, EKeyAtom::kWeaponArmed, true);
	ActionAddPrecondition(attackFromVehicle, EKeyAtom::kWeaponLoaded, true);
	ActionAddEffect(attackFromVehicle, EKeyAtom::kTargetIsDead, true);

	Action goTo = Action("GoTo", 2);
	ActionAddPrecondition(goTo, EKeyAtom::kDestination, true);
	ActionAddEffect(goTo, EKeyAtom::kAtObject, true);

	Action useObject = Action("UseObject", 2);
	ActionAddPrecondition(useObject, EKeyAtom::kAtObject, true);
	ActionAddEffect(useObject, EKeyAtom::kHasCover, true); // flips object to the side to create cover.

	Action equipWeapon = Action("EquipWeapon", 1);
	ActionAddEffect(equipWeapon, EKeyAtom::kWeaponArmed, true);

	Action reloadWeapon = Action("ReloadWeapon", 1);
	ActionAddPrecondition(reloadWeapon, EKeyAtom::kWeaponArmed, true);
	ActionAddEffect(reloadWeapon, EKeyAtom::kWeaponLoaded, true);

	// Basic movement and positioning actions
	Action sprint = Action("Sprint", 3);
	ActionAddEffect(sprint, EKeyAtom::kAtLocation, true);
	ActionAddEffect(sprint, EKeyAtom::kStamina, false);

	Action walk = Action("Walk", 5);
	ActionAddEffect(walk, EKeyAtom::kAtLocation, true);

	Action crouch = Action("Crouch", 1);
	ActionAddEffect(crouch, EKeyAtom::kIsCrouched, true);
	ActionAddEffect(crouch, EKeyAtom::kIsStealthy, true);

	Action prone = Action("Prone", 2);
	ActionAddEffect(prone, EKeyAtom::kIsProne, true);
	ActionAddEffect(prone, EKeyAtom::kIsStealthy, true);
	ActionAddEffect(prone, EKeyAtom::kAccuracyIncreased, true);

	// Cover and stealth actions
	Action findCover = Action("FindCover", 2);
	ActionAddPrecondition(findCover, EKeyAtom::kAtLocation, true);
	ActionAddEffect(findCover, EKeyAtom::kHasCover, true);

	Action enterBuilding = Action("EnterBuilding", 3);
	ActionAddPrecondition(enterBuilding, EKeyAtom::kAtLocation, true);
	ActionAddEffect(enterBuilding, EKeyAtom::kInsideBuilding, true);
	ActionAddEffect(enterBuilding, EKeyAtom::kHasCover, true);

	Action climbVantagePoint = Action("ClimbVantagePoint", 4);
	ActionAddPrecondition(climbVantagePoint, EKeyAtom::kAtLocation, true);
	ActionAddPrecondition(climbVantagePoint, EKeyAtom::kStamina, true);
	ActionAddEffect(climbVantagePoint, EKeyAtom::kHasHighGround, true);
	ActionAddEffect(climbVantagePoint, EKeyAtom::kAccuracyIncreased, true);

	// Weapon handling actions
	Action switchToSidearm = Action("SwitchToSidearm", 1);
	ActionAddEffect(switchToSidearm, EKeyAtom::kWeaponArmed, true);
	ActionAddEffect(switchToSidearm, EKeyAtom::kUsingPistol, true);

	Action switchToRifle = Action("SwitchToRifle", 2);
	ActionAddPrecondition(switchToRifle, EKeyAtom::kHasRifle, true);
	ActionAddEffect(switchToRifle, EKeyAtom::kWeaponArmed, true);
	ActionAddEffect(switchToRifle, EKeyAtom::kUsingRifle, true);

	Action switchToSniper = Action("SwitchToSniper", 2);
	ActionAddPrecondition(switchToSniper, EKeyAtom::kHasSniper, true);
	ActionAddEffect(switchToSniper, EKeyAtom::kWeaponArmed, true);
	ActionAddEffect(switchToSniper, EKeyAtom::kUsingSniper, true);

	Action reloadPistol = Action("ReloadPistol", 2);
	ActionAddPrecondition(reloadPistol, EKeyAtom::kUsingPistol, true);
	ActionAddPrecondition(reloadPistol, EKeyAtom::kHasPistolAmmo, true);
	ActionAddEffect(reloadPistol, EKeyAtom::kWeaponLoaded, true);

	Action reloadRifle = Action("ReloadRifle", 3);
	ActionAddPrecondition(reloadRifle, EKeyAtom::kUsingRifle, true);
	ActionAddPrecondition(reloadRifle, EKeyAtom::kHasRifleAmmo, true);
	ActionAddEffect(reloadRifle, EKeyAtom::kWeaponLoaded, true);

	Action reloadSniper = Action("ReloadSniper", 4);
	ActionAddPrecondition(reloadSniper, EKeyAtom::kUsingSniper, true);
	ActionAddPrecondition(reloadSniper, EKeyAtom::kHasSniperAmmo, true);
	ActionAddEffect(reloadSniper, EKeyAtom::kWeaponLoaded, true);

	// Combat actions
	Action quickShot = Action("QuickShot", 3);
	ActionAddPrecondition(quickShot, EKeyAtom::kWeaponArmed, true);
	ActionAddPrecondition(quickShot, EKeyAtom::kWeaponLoaded, true);
	ActionAddEffect(quickShot, EKeyAtom::kTargetIsDead, true);
	ActionAddEffect(quickShot, EKeyAtom::kWeaponLoaded, false);

	Action aimedShot = Action("AimedShot", 5);
	ActionAddPrecondition(aimedShot, EKeyAtom::kWeaponArmed, true);
	ActionAddPrecondition(aimedShot, EKeyAtom::kWeaponLoaded, true);
	ActionAddPrecondition(aimedShot, EKeyAtom::kAccuracyIncreased, true);
	ActionAddEffect(aimedShot, EKeyAtom::kTargetIsDead, true);
	ActionAddEffect(aimedShot, EKeyAtom::kWeaponLoaded, false);

	Action sniperShot = Action("SniperShot", 6);
	ActionAddPrecondition(sniperShot, EKeyAtom::kUsingSniper, true);
	ActionAddPrecondition(sniperShot, EKeyAtom::kWeaponLoaded, true);
	ActionAddPrecondition(sniperShot, EKeyAtom::kHasHighGround, true);
	ActionAddEffect(sniperShot, EKeyAtom::kTargetIsDead, true);
	ActionAddEffect(sniperShot, EKeyAtom::kWeaponLoaded, false);

	// Equipment actions
	Action searchForAmmo = Action("SearchForAmmo", 4);
	ActionAddEffect(searchForAmmo, EKeyAtom::kHasPistolAmmo, true);
	ActionAddEffect(searchForAmmo, EKeyAtom::kHasRifleAmmo, true);

	Action findRifle = Action("FindRifle", 3);
	ActionAddEffect(findRifle, EKeyAtom::kHasRifle, true);

	Action findSniper = Action("FindSniper", 5);
	ActionAddEffect(findSniper, EKeyAtom::kHasSniper, true);

	Action useStim = Action("UseStim", 2);
	ActionAddEffect(useStim, EKeyAtom::kStamina, true);

	// Vehicle actions
	Action enterVehicle = Action("EnterVehicle", 2);
	ActionAddPrecondition(enterVehicle, EKeyAtom::kAtLocation, true);
	ActionAddEffect(enterVehicle, EKeyAtom::kInVehicle, true);
	ActionAddEffect(enterVehicle, EKeyAtom::kHasCover, true);

	Action startVehicle = Action("StartVehicle", 1);
	ActionAddPrecondition(startVehicle, EKeyAtom::kInVehicle, true);
	ActionAddEffect(startVehicle, EKeyAtom::kVehicleRunning, true);

	Action driveVehicle = Action("DriveVehicle", 3);
	ActionAddPrecondition(driveVehicle, EKeyAtom::kInVehicle, true);
	ActionAddPrecondition(driveVehicle, EKeyAtom::kVehicleRunning, true);
	ActionAddEffect(driveVehicle, EKeyAtom::kAtLocation, true);

	// Support actions
	Action callSupport = Action("CallSupport", 4);
	ActionAddPrecondition(callSupport, EKeyAtom::kHasRadio, true);
	ActionAddEffect(callSupport, EKeyAtom::kSupportAvailable, true);

	Action markTarget = Action("MarkTarget", 2);
	ActionAddPrecondition(markTarget, EKeyAtom::kHasHighGround, true);
	ActionAddEffect(markTarget, EKeyAtom::kTargetMarked, true);

	Action callAirstrike = Action("CallAirstrike", 8);
	ActionAddPrecondition(callAirstrike, EKeyAtom::kSupportAvailable, true);
	ActionAddPrecondition(callAirstrike, EKeyAtom::kTargetMarked, true);
	ActionAddEffect(callAirstrike, EKeyAtom::kTargetIsDead, true);

	// Put all actions in the vector
	std::vector<Action*> actions;
	actions.push_back(&sprint);
	actions.push_back(&walk);
	actions.push_back(&crouch);
	actions.push_back(&prone);
	actions.push_back(&findCover);
	actions.push_back(&enterBuilding);
	actions.push_back(&climbVantagePoint);
	actions.push_back(&switchToSidearm);
	actions.push_back(&switchToRifle);
	actions.push_back(&switchToSniper);
	actions.push_back(&reloadPistol);
	actions.push_back(&reloadRifle);
	actions.push_back(&reloadSniper);
	actions.push_back(&quickShot);
	actions.push_back(&aimedShot);
	actions.push_back(&sniperShot);
	actions.push_back(&searchForAmmo);
	actions.push_back(&findRifle);
	actions.push_back(&findSniper);
	actions.push_back(&useStim);
	actions.push_back(&enterVehicle);
	actions.push_back(&startVehicle);
	actions.push_back(&driveVehicle);
	actions.push_back(&callSupport);
	actions.push_back(&markTarget);
	actions.push_back(&callAirstrike);
	// me
	actions.push_back(&attackFromCover);
	actions.push_back(&attack);
	actions.push_back(&attackFromVehicle);
	actions.push_back(&goTo);
	actions.push_back(&useObject);
	actions.push_back(&equipWeapon);
	actions.push_back(&reloadWeapon);

	WorldState currentState;
	BitsetInit(currentState.stateBits);
	
	BitsetWrite(currentState.stateBits, EKeyAtom::kTargetIsDead, false);
	BitsetWrite(currentState.stateBits, EKeyAtom::kWeaponArmed, false);
	BitsetWrite(currentState.stateBits, EKeyAtom::kWeaponLoaded, false);
	BitsetWrite(currentState.stateBits, EKeyAtom::kAtLocation, false);
	BitsetWrite(currentState.stateBits, EKeyAtom::kStamina, true);
	BitsetWrite(currentState.stateBits, EKeyAtom::kHasRadio, true);
	BitsetWrite(currentState.stateBits, EKeyAtom::kHasPistolAmmo, true);
	BitsetWrite(currentState.stateBits, EKeyAtom::kHasRifleAmmo, false);
	BitsetWrite(currentState.stateBits, EKeyAtom::kHasSniperAmmo, false);
	BitsetWrite(currentState.stateBits, EKeyAtom::kHasRifle, false);
	BitsetWrite(currentState.stateBits, EKeyAtom::kHasSniper, false);
	BitsetWrite(currentState.stateBits, EKeyAtom::kAccuracyIncreased, false);
	BitsetWrite(currentState.stateBits, EKeyAtom::kHasHighGround, false);
	BitsetWrite(currentState.stateBits, EKeyAtom::kHasCover, false);
	BitsetWrite(currentState.stateBits, EKeyAtom::kIsStealthy, false);

	RunPlannerBenchmark(killEnemy, currentState, actions);

	BitsetPrint(currentState.stateBits);
	std::vector<Action*> plan = Plan(killEnemy, currentState, actions);
	PlanPrint(plan);
	
	for (const Action* action : plan)
	{
		ActionApplyEffect(*action, currentState);
	}
	BitsetPrint(currentState.stateBits);

	return 0;
}
