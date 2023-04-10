#pragma once

// Character State Enum
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	//TODO: Making double for equipped and unequipped views. This is a temporary solution.
	IdleEquipped UMETA(DisplayName = "IdleEquipped"),
	IdleUnequipped UMETA(DisplayName = "IdleUnequipped"),
	WalkingEquipped UMETA(DisplayName = "WalkingEquipped"),
	WalkingUnequipped UMETA(DisplayName = "WalkingUnequipped"),
	SprintingEquipped UMETA(DisplayName = "SprintingEquipped"),
	SprintingUnequipped UMETA(DisplayName = "SprintingUnequipped"),
	// Doesn't matter
	Jumping UMETA(DisplayName = "Jumping"),
	Skating UMETA(DisplayName = "Skating"),
	Dead UMETA(DisplayName = "Dead"),
	Dodging UMETA(DisplayName = "Dodging"),
	// Can only be done equipped
	Attacking UMETA(DisplayName = "Attacking")
};

UENUM(BlueprintType)
enum class ECharacterStatusChange : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Sprinting UMETA(DisplayName = "Sprinting"),
	Jumping UMETA(DisplayName = "Jumping"),
	Skating UMETA(DisplayName = "Skating"),
	Dead UMETA(DisplayName = "Dead"),
	Dodging UMETA(DisplayName = "Dodging"),
	Attacking UMETA(DisplayName = "Attacking")
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	Unoccupied UMETA(DisplayName = "Unoccupied"),
	Attacking UMETA(DisplayName = "Attacking"),
	Interacting UMETA(DisplayName = "Interacting"),
	Dodging UMETA(DisplayName = "Dodging"),
	Blocking UMETA(DisplayName = "Blocking"),
	Parrying UMETA(DisplayName = "Parrying"),
	Stunned UMETA(DisplayName = "Stunned"),
	Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	Alive UMETA(DisplayName = "Alive"),
	Death1 UMETA(DisplayName= "Death1"),
	Death2 UMETA(DisplayName= "Death2"),
	//Death3 UMETA(DisplayName= "Death3")
};


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Dead UMETA(DisplayName = "Dead"),
	Patrolling UMETA(DisplayName = "Patrolling"),
	Chasing UMETA(DisplayName= "Chasing"),
	Attacking UMETA(DisplayName= "Attacking"),
	Engaged UMETA(DisplayName = "Engaged")
};