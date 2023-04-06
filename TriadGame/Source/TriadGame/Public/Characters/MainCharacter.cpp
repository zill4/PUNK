// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
// Input
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
// Camera
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
// Movement
#include "GameFramework/CharacterMovementComponent.h"
// Groom
#include "GroomComponent.h"
// Items
#include "Item.h"
#include "Weapon.h"
#include "MovementItem.h"
// Animations
#include "Animation/AnimMontage.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// Camera + SpringArm
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 250.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	// Groom
	/*Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");*/
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMCContext, 0);
		}
	}
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMainCharacter::Jump()
{
	Super::Jump();
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	float MovementMagnitude = 1.f;
	// Slow down movement if attacking
	if (ActionState == ECharacterActionState::Attacking)
		MovementMagnitude = .25f;

		const FVector2D MovementVector = Value.Get<FVector2D>() * MovementMagnitude;

		//const FVector Forward = GetActorForwardVector();
		//const FVector Right = GetActorRightVector();

		//AddMovementInput(Forward, MovementVector.Y);
		//AddMovementInput(Right, MovementVector.X);

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);


		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	
	
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AMainCharacter::StartSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	bIsSprinting = true;
	ChangeState(ECharacterStatusChange::Sprinting);
}

void AMainCharacter::StopSprint(const FInputActionValue& Value)
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	ChangeState(ECharacterStatusChange::Walking);
}

void AMainCharacter::Interact(const FInputActionValue& Value)
{
	// TODO: Refactor for all overlap events
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		EquippedWeapon = OverlappingWeapon;
		bWeaponDrawn = true;
		// TODO: This is stupid the state change setup is stupid, needs to be fixed
		ChangeState(ECharacterStatusChange::Idle);
	}
	else
	{
		PlayEquipWeaponMontage(FName("Unequip"));
		CharacterState = ECharacterState::IdleUnequipped;
		//if (bWeaponDrawn)
		//{
		//}
		if (CanEquipWeapon())
		{
			PlayEquipWeaponMontage(FName("Equip"));
			CharacterState = ECharacterState::IdleEquipped;
		}
		
	}

	AMovementItem* OverlappingMovementItem = Cast<AMovementItem>(OverlappingItem);
	if (OverlappingMovementItem)
	{
		GetCharacterMovement()->MaxWalkSpeed = 1000.f;
		OverlappingMovementItem->Equip(GetMesh(), FName("foot_l_Socket"));
		ChangeState(ECharacterStatusChange::Skating);
	}
}

void AMainCharacter::Attack(const FInputActionValue& Value)
{
	if (ActionState != ECharacterActionState::Attacking && bWeaponDrawn)
	{
		ActionState = ECharacterActionState::Attacking;
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			if (AttackCombo == 0) // Only Start the timer if we are at 0
				GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &AMainCharacter::ResetCombo, ComboCooldown, true);

			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName(*FString::Printf(TEXT("Attack%d"), AttackCombo)), AttackMontage);

			UE_LOG(LogTemp, Warning, TEXT("AttackCombo: Attack%d"), AttackCombo)
			AttackCombo = (AttackCombo + 1) % 3;

			//if (CharacterState == ECharacterState::Equipped2HWeapon)
			//{
			//	CharacterState = ECharacterState::Attacking;
			//}
		}
	}
}

void AMainCharacter::ResetCombo()
{
	AttackCombo = 0;
	StopCombo();
}

bool AMainCharacter::CanUnequipWeapon()
{
	return (ActionState == ECharacterActionState::Unoccupied && bWeaponDrawn && EquipWeaponMontage && EquippedWeapon);
}

bool AMainCharacter::CanEquipWeapon()
{
	return (ActionState != ECharacterActionState::Unoccupied && !bWeaponDrawn && EquipWeaponMontage && EquippedWeapon);
}

void AMainCharacter::PlayEquipWeaponMontage(FName SectionName)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(EquipWeaponMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipWeaponMontage);
	}
}

void AMainCharacter::StopCombo()
{
	GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
}

void AMainCharacter::StopAttack(const FInputActionValue& Value)
{
	//if (CharacterState == ECharacterState::Attacking)
	//{
	//	CharacterState = ECharacterState::Equipped2HWeapon;
	//	// Would reset AttackCombo here, but it should be based on time not last played
	//}
}

void AMainCharacter::ChangeState(ECharacterStatusChange DesiredStatus)
{
	// Basically if state is equipped
	if (bWeaponDrawn)
	{
		switch (DesiredStatus)
		{
		case ECharacterStatusChange::Idle:
			CharacterState = ECharacterState::IdleEquipped;
		break;
		case ECharacterStatusChange::Walking:
			CharacterState = ECharacterState::WalkingEquipped;
		break;
		case ECharacterStatusChange::Sprinting:
			CharacterState = ECharacterState::SprintingEquipped;
		break;
		case ECharacterStatusChange::Jumping:
			CharacterState = ECharacterState::Jumping;
		break;
		case ECharacterStatusChange::Skating:
			CharacterState = ECharacterState::Skating;
		break;
		case ECharacterStatusChange::Dead:
			CharacterState = ECharacterState::Dead;
		break;
		case ECharacterStatusChange::Dodging:
			CharacterState = ECharacterState::Dodging;
		break;
		case ECharacterStatusChange::Attacking:
			CharacterState = ECharacterState::Attacking;
		break;
		default: CharacterState = ECharacterState::IdleEquipped;
		}
	}
	else
	{
		switch (DesiredStatus)
		{
		case ECharacterStatusChange::Idle:
			CharacterState = ECharacterState::IdleUnequipped;
			break;
		case ECharacterStatusChange::Walking:
			CharacterState = ECharacterState::WalkingUnequipped;
			break;
		case ECharacterStatusChange::Sprinting:
			CharacterState = ECharacterState::SprintingUnequipped;
			break;
		case ECharacterStatusChange::Jumping:
			CharacterState = ECharacterState::Jumping;
			break;
		case ECharacterStatusChange::Skating:
			CharacterState = ECharacterState::Skating;
			break;
		case ECharacterStatusChange::Dead:
			CharacterState = ECharacterState::Dead;
			break;
		case ECharacterStatusChange::Dodging:
			CharacterState = ECharacterState::Dodging;
			break;
		case ECharacterStatusChange::Attacking:
			CharacterState = ECharacterState::Attacking;
			break;
		default: CharacterState = ECharacterState::IdleUnequipped;
		}
	}
}

void AMainCharacter::AttackEnd()
{
	ActionState = ECharacterActionState::Unoccupied;
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jump);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMainCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMainCharacter::StopSprint);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started , this, &AMainCharacter::Interact);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMainCharacter::Attack);
		//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AMainCharacter::StopAttack);
	}
}

