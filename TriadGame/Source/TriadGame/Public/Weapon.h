// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class TRIADGAME_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	void Equip(USceneComponent* InParent, FName InSocketName);

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	TArray<AActor*> IgnoreActors;
protected:

	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// Collision
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Transient Fields for distructibles
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

private:

	//TODO: Add equip weapon sound USoundBase
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* WeaponCollision;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent *BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent *BoxTraceEnd;

	

public:
	FORCEINLINE UBoxComponent* GetWeaponCollision() const { return WeaponCollision; }
	FORCEINLINE USceneComponent* GetBoxTraceStart() const { return BoxTraceStart; }
	FORCEINLINE USceneComponent* GetBoxTraceEnd() const { return BoxTraceEnd; }
};
