// Michael Adaixo - 2025

#pragma once
#include <unordered_map>

enum class EKeyAtom : uint64_t {
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

struct NoCopy
{
protected:
	NoCopy() = default;
	~NoCopy() = default;

private:
	NoCopy(const NoCopy&) = delete;
	NoCopy& operator=(const NoCopy&) = delete;
};

