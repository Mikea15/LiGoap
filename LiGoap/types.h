// Michael Adaixo - 2025

#pragma once

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

struct Fact
{
	EKeyAtom Key;
	bool value;

	inline bool operator==(const Fact& Other) const { return Key == Other.Key; } 
};

namespace std
{
	template<>
	struct hash<Fact>
	{
		size_t operator()(const Fact& fact) const noexcept
		{
			return hash<unsigned long>()(static_cast<unsigned long>(fact.Key));
		}
	};
}


using key_type = EKeyAtom;
using key_map = std::unordered_map<key_type, bool>;

