// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacterAnimationInstance.h"
#include "Characters/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainCharacterAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	if (MainCharacter)
	{
		CharacterMovement = MainCharacter->GetCharacterMovement();
		AttackCombo = MainCharacter->GetAttackCombo();
	}
}

void UMainCharacterAnimationInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
		bIsFalling = CharacterMovement->IsFalling();
		CharacterState = MainCharacter->GetCharacterState();
		CharacterActionState = MainCharacter->GetCharacterActionState();
		DeathPose = MainCharacter->GetDeathPose();
	}
}
