// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "InputActionValue.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UAnimMontage;
//class USoundBase;

UCLASS()
class TRIADGAME_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;
	virtual void Attack(const FInputActionValue& Value);
	virtual bool CanAttack();
	virtual void Die();
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	void DirectionalHitReaction(const FVector& ImpactPoint);
	void PlayHitReactMontage(const FName& SectionName);


	// Weapon
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* EquippedWeapon;

	// Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAttributeComponent* Attributes;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* HitParticles;
public:	

};
