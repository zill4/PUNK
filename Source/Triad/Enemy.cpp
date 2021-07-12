// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Main.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"
#include "Combat.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsAggroOverlapping = false;
	bIsCombatOverlapping = false;
	bIsAttacked = false;

	AggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
	AggroSphere->SetupAttachment(GetRootComponent());
	AggroSphere->InitSphereRadius(600.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);


	// Enemy stats
	Health = 100;
	Mana = 100;
	Stamina = 100;
	Name = "Sally";
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());

	AggroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AggroSphereOnOverlapBegin);
	AggroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AggroSphereOnOverlapEnd);


	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	OnActorHit.AddDynamic(this, &AEnemy::EnemyActorHit);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Dead boi
	if (Health <= 0)
		Die();
}


// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemy::AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			MainRef = Main;
			MoveToTarget(Main);
			// Will need to initiate combat here...
			CombatInstance->InitiateCombat(this, Main);
		}
	}
	// Aggro sphere is overlapping...
	bIsAggroOverlapping = true;
}

void AEnemy::AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{


	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			// Enemy Shoul begin idling if player character is outside of aggro sphere.
			// *Note* May want to add another aggro sphere. A chase sphere and a notice sphere, could build into greater AI of Enemy.
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			AIController->StopMovement();
		}
	}
	// Aggro sphere is not overlapping...
	bIsAggroOverlapping = false;
}

/**
*	Combat Sphere on Overlap Begin
*	Once the Weapon is colliding with the combat sphere of the enemy, checks for player attacking, and takes damage. Also attacks.
*	TODO: Fix attacking sequence to be a bit more dynamic.
*/
void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		// If Player is attacking and sword is overlapping combat sphere.
		AWeapon* Weapon = Cast<AWeapon>(OtherActor);
		if (Weapon)
		{
			// Set other status to combat & Set Move status to attacking
			SetEnemyStatus(EEnemyStatus::ESS_Combat);
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);	

			//if (MainRef->bAttacking)
				//Hit(); // Enemy was hit.
		
			
		}
	}
	// Combat Sphere is not overlapping
	bIsCombatOverlapping = true;
}
/**
*	Combat Sphere on Overlap End
*	After the detected object leaves the combat sphere.
*	TODO: Switch combat sphere detection from Main to Weapon and maybe use for attack sequencing, animations, and particles.
*/
void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			// Enemy should imediately move to the the target as soon as overlap ends.
			MoveToTarget(Main);
			if (Main->GetPlayerStatus() == EPlayerStatus::EPS_Attacking)
				bIsAttacked = true;
			else
				bIsAttacked = false;
		}
		// If the spider has died....
	
	}
	bIsCombatOverlapping = false;
}
/**
*	Move to Target
*	Enemy will move to detected target using Navpath
*	TODO: Add more dynamic pathing?
*/
void AEnemy::MoveToTarget(AMain* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		FNavPathSharedPtr NavPath;

		// Movement AI Struct Settings
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(5.0f);
		
		AIController->MoveTo(MoveRequest, &NavPath);
		
		/***
		* Debug Code
		*
		* 		
			TArray<FNavPathPoint> PathPoints = NavPath->GetPathPoints();
			
			for (auto Point : PathPoints)
			{
				FVector Location = Point.Location;
				UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, 1.5f);
			}
			UE_LOG(LogTemp, Warning, TEXT("Moving To Target"));
		*
		*/

	}
}
void AEnemy::EnemyActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	IsHit();
}
/**
*	Hit
*	When the enemy is hit, deduct health points, and play particle effect.
*	TODO: Maybe add rage counter, or some tracker for more dynamic AI.
*/
void AEnemy::IsHit()
{
	Health -= 10;
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	if (OverlapParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
	}
	if (OverlapSound)
	{
		UGameplayStatics::PlaySound2D(this, OverlapSound);
	}
}

/**
*	Die
*	Enemy dies, set the status to dead (not sure if it matters...)
*	Destroy Enemy, maybe I can trigger the death animation from here...
*	TODO: Add loot drops, from a loot table for this specified enemy.
*/
void AEnemy::Die()
{
	// Probably should add other stuff like loot and what have you.
	SetEnemyStatus(EEnemyStatus::ESS_Dead);
	Destroy();
}

