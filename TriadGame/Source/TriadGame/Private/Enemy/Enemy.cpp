// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Weapon.h"
#include "TriadGame\DebugMacros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	HealthBar = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationPitch = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthBar)
	{
		HealthBar->SetHealthPercent(1.f);
		HealthBar->SetVisibility(false);
	}

	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState > EEnemyState::Patrolling)
	{
		CheckCombatTarget();
	}
	else 
	{
		// Get next patrol target
		CheckPatrolTarget();
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float PatrolWaitTime = FMath::FRandRange(PatrolWaitTimeMin, PatrolWaitTimeMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, PatrolWaitTime);
	}
}

void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		CombatTarget = nullptr;
		if (HealthBar)
		{
			HealthBar->SetVisibility(false);
		}
		EnemyState = EEnemyState::Patrolling;
		// TODO: Add walk speed variables
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToTarget(PatrolTarget);
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::Chasing)
	{
		// Outside attack range, chase target
		EnemyState = EEnemyState::Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::Attacking)
	{
		// Inside attack range, attack target
		EnemyState = EEnemyState::Attacking;
		// TODO: attack montage
		Attack(NULL);
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (Attributes && Attributes->IsAlive())
	{
		DirectionalHitReaction(ImpactPoint);
	}
	else
	{
		Die();
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}
	if (HealthBar)
	{
		HealthBar->SetVisibility(true);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBar)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBar->SetHealthPercent(Attributes->GetHealthPercent());
	}
	CombatTarget = EventInstigator->GetPawn();
	if (EnemyState != EEnemyState::Chasing)
	{
		EnemyState = EEnemyState::Chasing;
		// TODO: refactor logic into a function
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
	}
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}


void AEnemy::Die()
{
	if (HealthBar)
	{
		HealthBar->SetVisibility(false);
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		FName SectionName = FName();
		const int32 Selection = FMath::RandRange(0, 1);

		switch (Selection)
		{
			case 0:
				SectionName = FName("Death1");
				DeathPose = EDeathPose::Death1;
				break;
			case 1:
				SectionName = FName("Death2");
				DeathPose = EDeathPose::Death2;
				break;
			//case 2:
			//	SectionName = FName("Death3");
			//	DeathPose = EDeathPose::Death3;
			//	break;
			default:
				break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
}

void AEnemy::Attack(const FInputActionValue& Value)
{
	Super::Attack(Value);

	// Play attack montage
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		FName SectionName = FName();
		const int32 Selection = FMath::RandRange(0, 2);
		switch (Selection)
		{
			case 0:
				SectionName = FName("Attack1");
				break;
			case 1:
				SectionName = FName("Attack2");
				break;
			case 2:
				SectionName = FName("Attack2");
				break;
			default:
				break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}


void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (EnemyState == EEnemyState::Chasing) return;
	if (SeenPawn->ActorHasTag(FName("Player")))
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = SeenPawn;
		if (EnemyState != EEnemyState::Attacking)
		{
			EnemyState = EEnemyState::Chasing;
			MoveToTarget(CombatTarget);
		}
	}
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_SPHERE_SingleFrame(Target->GetActorLocation());
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	// Get next patrol target
	TArray<AActor*> ValidTargets;
	for (auto Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];;
	}
	return nullptr;
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}
