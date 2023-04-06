// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "MainCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class TRIADGAME_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	virtual void Jump() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	//UFUNCTION()
	//void DisableWeaponCollision();

protected:
	virtual void BeginPlay() override;

	// Input Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bIsSprinting = false;


	// Input Actions
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMCContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	// Input Functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void StopAttack(const FInputActionValue& Value);

	// Attack Related
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	int AttackCombo = 0;

	UPROPERTY()
	FTimerHandle ComboResetTimerHandle;

	UPROPERTY()
	float ComboCooldown = 3.5f;

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	bool bWeaponDrawn = false;

	// Returns the proper state if the character is holding a weapon
	void ChangeState(ECharacterStatusChange DesiredStatus);

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void EnterUnoccupied();

private:
	// Camera
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	// Groom Components
	//UPROPERTY(VisibleAnywhere, Category = Hair)
	//UGroomComponent* Hair;

	//UPROPERTY(VisibleAnywhere, Category = Hair)
	//UGroomComponent* Eyebrows;

	// Character State
	ECharacterState CharacterState = ECharacterState::IdleUnequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	ECharacterActionState ActionState = ECharacterActionState::Unoccupied;

	// Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipWeaponMontage;

	void PlayEquipWeaponMontage(const FName& SectionName);

	// Attack Combo
	void StopCombo();
	void ResetCombo();

	// Status checks
	bool CanUnequipWeapon();
	bool CanEquipWeapon();

	// Weapon
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* EquippedWeapon;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE int GetAttackCombo() const { return AttackCombo; }
};
