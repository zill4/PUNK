// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking UMETA(DisplayName = "Attacking"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};


UENUM(BlueprintType)
enum class EEnemyStatus : uint8
{
	ESS_Job UMETA(DisplayName = "Job"),
	ESS_Combat UMETA(DisplayName = "Combat"),
	ESS_Dead UMETA(DisplayName = "Dead"),

	ESS_MAX UMETA(DisplayName = "DefaultMAX")
};


UCLASS()
class TRIAD_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EEnemyMovementStatus EEnemeyMovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyStats")
	EEnemyStatus CurrentEEnemyStatus;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus _Status) { EEnemeyMovementStatus = _Status;  }
	FORCEINLINE void SetEnemyStatus(EEnemyStatus _Status) { CurrentEEnemyStatus = _Status; }
	/**
		Overlap event triggers
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* AggroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* CombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bIsCombatOverlapping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bIsAggroOverlapping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bIsAttacked;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	AMain* MainRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	class UParticleSystem* OverlapParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* OverlapSound;
	/**
		Enemy Base Stats and properties
		Health: Defines how dead or alive the enemy is...
		Stamina: Defines how much stamina... the enemy has to use abilities
		Mana: Defines how much mana the Enemy has to use on its abilities
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float Health;
	// Maybe not necessary
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	FString Name;

	class Combat* CombatInstance;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// A virtual class can be overwitten by child classes.
	UFUNCTION()
	virtual void AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// A virtual class can be overwitten by child classes.
	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void MoveToTarget(class AMain* Target);

	UFUNCTION()
	void Die();

	UFUNCTION()
	void IsHit();

	UFUNCTION()
	FORCEINLINE float GetHealth() { return (Health); }

	UFUNCTION()
	virtual void EnemyActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};