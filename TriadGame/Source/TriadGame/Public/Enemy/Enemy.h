// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;
class USoundBase;
class UAttributeComponent;
class UWidgetComponent;
UCLASS()
class TRIADGAME_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
protected:
	virtual void BeginPlay() override;

	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReaction(const FVector& ImpactPoint);
private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere, Category = "HUD")
	UWidgetComponent* HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* HitParticles;
public:	
};
