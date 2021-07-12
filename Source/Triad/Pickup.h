// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class TRIAD_API APickup : public AItem
{
	GENERATED_BODY()
	
public:

	APickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coins")
	int32 CoinCount;
public:
	/** Override Classes, these will not have the UFUNCTION Macros as they are inherited.*/
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
