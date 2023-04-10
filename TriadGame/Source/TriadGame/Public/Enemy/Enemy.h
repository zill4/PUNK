// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;
UCLASS()
class TRIADGAME_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	void CheckPatrolTarget();
	void CheckCombatTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	virtual void Die() override;
	virtual void Attack(const FInputActionValue& Value) override;

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);

	AActor* ChoosePatrolTarget();

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::Alive;

private:

	UPROPERTY(VisibleAnywhere, Category = "HUD")
	UHealthBarComponent* HealthBar;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensing;
	
	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 160.f;

	UPROPERTY(EditAnywhere)
	double AcceptanceRadius = 80.f;

	UPROPERTY()
	AAIController* EnemyController;

	/*
	*  Navigation
	*/
	// Current Patrol Target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	TArray<AActor*>PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;


	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitTimeMin = 2.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitTimeMax = 5.f;

	EEnemyState EnemyState = EEnemyState::Patrolling;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
public:	
};
