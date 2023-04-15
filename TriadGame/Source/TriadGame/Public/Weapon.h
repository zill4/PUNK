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

	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	TArray<AActor*> IgnoreActors;

	void DisableSphereCollision();
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

	// TODO: this is hard coded for enemy type, need to change that to be more generic
	bool ActorIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& HitResult);

private:

	void BoxTrace(FHitResult& HitResult);

	//TODO: Add equip weapon sound USoundBase
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* WeaponCollision;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent *BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent *BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	bool bShowBoxDebug = false;

public:
	FORCEINLINE UBoxComponent* GetWeaponCollision() const { return WeaponCollision; }
	FORCEINLINE USceneComponent* GetBoxTraceStart() const { return BoxTraceStart; }
	FORCEINLINE USceneComponent* GetBoxTraceEnd() const { return BoxTraceEnd; }
};
