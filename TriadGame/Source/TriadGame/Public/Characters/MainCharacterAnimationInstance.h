// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "MainCharacterAnimationInstance.generated.h"

class AMainCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class TRIADGAME_API UMainCharacterAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	AMainCharacter* MainCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly)
	int AttackCombo;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Action State")
	ECharacterActionState CharacterActionState;

	UPROPERTY(BlueprintReadOnly, Category = "Action State")
	TEnumAsByte<EDeathPose> DeathPose;
};
