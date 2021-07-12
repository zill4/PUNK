// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Enemy.h"
#include "Animation/AnimTypes.h"
#include "Combat.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CombatSphere for detecting when player is within attack range.
	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);

	// Pulls towards the player if there is a collision
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; // Camera follows at this distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate arm based on controller. Attached to pawn and moves along with pawn.

	// Set Size for collision Capsule
	GetCapsuleComponent()->SetCapsuleSize(48.f, 105.f);
												// Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // A socket allows you to attach things to parts of the object.
	// Attach Camera to end of boom and let the boom adjust to match
	// the controller orientation
	FollowCamera->bUsePawnControlRotation = false; 

	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	// Don't rotate when the controller rotates.
	// Just affect the camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Character will automaticaly turn towards direction it is moving
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Player Stats

	 MaxHealth = 100.f;
	 Health = 65.f;
	 MaxStamina = 150.f;
	 Stamina = 120.f;
	 Coins = 0;
	 RunningSpeed = 650.f;
	 SprintingSpeed = 950.f;
	 StaminaDrainRate = 25.f;
	 MinSprintStamina = 50.f;

	 // KeyPress booleans

	 bShiftKeyDown = false;
	 bInteractDown = false;
	 bAttackDown = false;
	 bBlockDown = false;

	 // Initialize Enums
	 MovementStatus = EMovementStatus::EMS_Normal;
	 StaminaStatus = EStaminaStatus::ESS_Normal;

	 CombatCounter = 1;
	 Name = "Bob";
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = GetMesh()->GetAnimInstance();
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Sprinting(DeltaTime);
}



/**
*	Movement and Key Press related Function
*
*/

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	/**
	*	Action Mappings
	*/
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMain::InteractDown);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AMain::InteractUp);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMain::AttackDown);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMain::AttackUp);

	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AMain::BlockDown);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AMain::BlockUp);

	/**
	*	Axis Mappings
	*/
	PlayerInputComponent->BindAxis("MoveY", this, &AMain::MoveY);
	PlayerInputComponent->BindAxis("MoveX", this, &AMain::MoveX);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);



}

void AMain::MoveY(float value)
{

	if ((Controller != nullptr) && (value != 0.0f))
	{
		// Find the forward direction
		if (!AnimInstance->Montage_IsPlaying(CombatMontage))
		{
			RunningSpeed = 650.f;

			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, value);
			CombatCounter = 0; // Reset the combo for combat
		}
	}
}

void AMain::MoveX(float value)
{
	if ((Controller != nullptr) && (value != 0.0f))
	{
		if (!AnimInstance->Montage_IsPlaying(CombatMontage))
		{
			RunningSpeed = 650.f;

			// Find the forward direction
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, value);
			CombatCounter = 0; // Reset the combo for combat
		}
	}
}

void AMain::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMain::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

}

void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

/** Interact Button E */
void AMain::InteractDown()
{
	bInteractDown = true;
	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
}
void AMain::InteractUp()
{
	bInteractDown = false;
}

/** Attack Button LMB */
void AMain::AttackDown()
{
	bAttackDown = true;
	if (EquippedWeapon)
	{
		Attack();
		UE_LOG(LogTemp, Error, TEXT("Attacking"));
	}
}
void AMain::AttackUp()
{
	bAttackDown = false;
	SetPlayerStatus(EPlayerStatus::EPS_Normal);
	bAttacking = false;
}

void AMain::Attack()
{
	if (AnimInstance && CombatMontage)
	{
		// Don't start a new animation if the current animation isn't finished.
		if (!AnimInstance->Montage_IsPlaying(CombatMontage))
		{
			RunningSpeed = 0.f;
			SetPlayerStatus(EPlayerStatus::EPS_Attacking);
			bAttacking = true;
			AnimInstance->Montage_Play(CombatMontage, 1.45f);
			
				switch (CombatCounter)
				{
				case 1: AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
					break;
				case 2: AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
					break;
				case 3: AnimInstance->Montage_JumpToSection(FName("Attack_3"), CombatMontage);
						
					break;
				default: AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				}
				CombatCounter %= 4;
				CombatCounter += 1;
		}
		else
		{
			SetPlayerStatus(EPlayerStatus::EPS_Normal);
			bAttacking = false;
		}
	}
}


/** Block Button RMB */
void AMain::BlockDown()
{
	bBlockDown = true;
}
void AMain::BlockUp()
{
	bBlockDown = false;
}


/**
*	Character Stats and Status functions
*
*/
void AMain::DecrementHealth(float Amount)
{
	Health -= Amount;
	if (Health <= 0.f)
		Die();
}

void AMain::Die()
{
	Destroy();
}

void AMain::IncrementCoin(int32 Amount)
{
	Coins += Amount;
}

void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
		if (MovementStatus == EMovementStatus::EMS_Sprinting)
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
		}
}

/***
* Debug related Functions
*
*/
void AMain::ShowPickupLocations()
{
	for (int32 i = 0; i < PickupLocations.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, PickupLocations[i], 25.f, 8, FLinearColor::Red, 10.f, 0.5f);
	}

}

void AMain::Sprinting(float DeltaTime)
{
	// If this were not a time crunch I would create specific playerStats class to define this functionality.
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown && !AnimInstance->Montage_IsPlaying(CombatMontage))
		{
			if (Stamina - DeltaStamina <= MinSprintStamina)
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			Stamina -= DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else
		{
			if (Stamina + DeltaStamina >= MaxStamina)
				Stamina = MaxStamina;
			else
				Stamina += DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown && !AnimInstance->Montage_IsPlaying(CombatMontage))
		{
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				Stamina -= DeltaStamina;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown && !AnimInstance->Montage_IsPlaying(CombatMontage))
		{
			Stamina = 0.f;
		}
		else
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina + DeltaStamina >= MinSprintStamina)
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
		Stamina += DeltaStamina;
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		;
	}
}

void AMain::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AMain::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}