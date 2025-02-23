// Michael Adaixo - 2025

#pragma once

enum class EKeyAtom : unsigned long {
	Empty = 0,

	kTargetIsDead,
	kWeaponArmed,
	kWeaponLoaded,
	kHasCover,
	kAtObject,
	kAtLocation,
	kStamina,
	kHasRadio,
	kHasPistolAmmo,
	kHasRifleAmmo,
	kHasSniperAmmo,
	kHasRifle,
	kHasSniper,
	kAccuracyIncreased,
	kHasHighGround,
	kIsStealthy,
	kSupportAvailable,
	kTargetMarked,
	kDestination,
	kIsCrouched,
	kIsProne,
	kInsideBuilding, 
	kUsingPistol,
	kUsingRifle,
	kUsingSniper,
	kInVehicle,
	kVehicleRunning,
	
	Count
};


using key_type = EKeyAtom;
using key_map = std::unordered_map<key_type, bool>;

