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

struct Symbol
{
	union
	{
		float fValue;
		int iValue;
		bool bValue;
		void* ptrValue = nullptr;
	};
};

struct Fact
{
	EKeyAtom key;
	Symbol value;

	inline bool operator==(const Fact& Other) const { return key == Other.key; } 
};

template<>
struct std::hash<Fact>
{
	size_t operator()(const Fact& fact) const noexcept
	{
		return hash<unsigned long>()(static_cast<unsigned long>(fact.key));
	}
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

