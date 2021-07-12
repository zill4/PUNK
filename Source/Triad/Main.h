// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

// use class for scoping, UENUM makes it blueprintable
UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),
	
	ESS_MAX UMETA(DisplayName = "DefaultMax")

};

UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	EPS_Normal UMETA(DisplayName = "Normal"),
	EPS_Attacking UMETA(DisplayName = "Attacking"),

	EPS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class TRIAD_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();
	
	// Template Container
	TArray<FVector> PickupLocations;
	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	/** Enum States */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EPlayerStatus PlayerStatus;

	/** Cameara boom positioning the camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"));
	class USpringArmComponent* CameraBoom;

	/** Follow Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"));
	class UCameraComponent* FollowCamera;

	/** Base Turn rate to scale turning fuctions for the Camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/***/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	bool bShiftKeyDown;
	bool bInteractDown;
	bool bAttackDown;
	bool bBlockDown;

	// OUTDATED: Change to Attacking State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bAttacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	class UAnimInstance* AnimInstance;

	class Combat* CombatInstance;
	
	int16 CombatCounter;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* CombatSphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forward and backward movement*/
	void MoveY(float value);

	/** Called for left and right movement*/
	void MoveX(float value);

	/** Called via input to turn at a given rate
	* @param Rate This is a normalized rate, i.e. 1.0 is 100% of desired turn rate.
	*/
	void TurnAtRate(float Rate);

	/** Called via input to look up at a given rate
	* @param Rate This is a normalized rate, i.e. 1.0 is 100% of desired turn rate.
	*/
	void LookUpAtRate(float Rate);

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Player Stat Functions*/
	UFUNCTION()
	void DecrementHealth(float Amount);

	UFUNCTION()
	void Die();

	UFUNCTION()
	void IncrementCoin(int32 Amount);

	/** Set Movement Status and Running speed*/
	void SetMovementStatus(EMovementStatus Status);

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	/** Set/Get Status for Player, Attacking, Normal, Maybe... Poisoned or some other effects */
	FORCEINLINE void SetPlayerStatus(EPlayerStatus Status) { PlayerStatus = Status; }
	FORCEINLINE EPlayerStatus GetPlayerStatus() { return (PlayerStatus); }

	/** Down to start sprinting */
	void ShiftKeyDown();
	/** Up to stop Sprinting*/
	void ShiftKeyUp();

	/** Interact Button E */
	void InteractDown();
	void InteractUp();

	/** Attack Button LMB */
	void AttackDown();
	void AttackUp();

	void Attack();
	FORCEINLINE bool IsAttacking() { return(bAttacking); }



	/** Block Button RMB */
	void BlockDown();
	void BlockUp();
	
	/** Weapon Equip Functions*/
	FORCEINLINE void SetWeapon(class AWeapon* _Weapon) { EquippedWeapon = _Weapon; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AWeapon* GetWeapon() { return(EquippedWeapon); }
	FORCEINLINE void SetActiveOverlappingItem(class AItem* _Item) { ActiveOverlappingItem = _Item; }

	UFUNCTION()
	void Sprinting(float DeltaTime);

	/* Minimum Combat Range Logic*/
	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	/**
	*
	*	Player Stats
	*
	*
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStats")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float Stamina;

	// Cross platform safegaurd for integers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	int32 Coins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | PlayerStats")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | PlayerStats")
	float SprintingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | PlayerStats")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | PlayerStats")
	float MinSprintStamina;

	/** Weapon Equip*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items | PlayerStats")
	class AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items | PlayerStats")
	class AItem* ActiveOverlappingItem;

};
